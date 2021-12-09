#include "GameFramework/Weapon/TaskPlayerWeapon.h"

#include "GeneratedCodeHelpers.h"
#include "GameFramework/Weapon/TaskProjectile.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"

UTaskPlayerWeapon::UTaskPlayerWeapon()
{
	
}


void UTaskPlayerWeapon::OnStartFireWeapon()
{
	if(ProjectileClass && IAmmo>0 && !bisReloading)
	{
		GetWorld()->GetTimerManager().SetTimer(FireHandle,this,&UTaskPlayerWeapon::OnFire,FFireRate,true);
		OnFire();
		OnWeaponFireDelegate.Broadcast();
	}
}


void UTaskPlayerWeapon::OnFire()
{
	if (IAmmo<1)
	{
		GetWorld()->GetTimerManager().ClearTimer(FireHandle);
		FireHandle.Invalidate();
		Server_ReloadWeapon();
		OnWeaponFireDelegate.Broadcast();
		return;
	}

	if (OwnerPlayer)
	{
		const FVector Start=OwnerPlayer->GetFirstPersonCameraComponent()->GetComponentLocation();
		const FVector End=(OwnerPlayer->GetFirstPersonCameraComponent()->GetForwardVector()*10000000)+Start;
		FHitResult PlayerCameraHitResult;
		GetWorld()->LineTraceSingleByChannel(PlayerCameraHitResult,Start,End,TraceChannel);

		FHitResult WeaponHitResult;
		GetWorld()->LineTraceSingleByChannel(WeaponHitResult,GetSocketLocation(FireSocketName),PlayerCameraHitResult.TraceEnd,TraceChannel);

		FTransform WeaponFireTransform(UKismetMathLibrary::FindLookAtRotation(WeaponHitResult.TraceStart,WeaponHitResult.TraceEnd),WeaponHitResult.TraceStart,FVector(1,1,1));

		Server_OnFire(WeaponFireTransform,WeaponHitResult);
		OnWeaponFireDelegate.Broadcast();
	}

}

void UTaskPlayerWeapon::Server_OnFire_Implementation(const FTransform SpawnTransform,FHitResult ImpactResult)
{
	if (ProjectileClass)
	{
		FActorSpawnParameters ActorSpawnParams;
		ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
		GetWorld()->SpawnActor<ACTFTaskProjectile>(ProjectileClass, SpawnTransform.GetLocation(), SpawnTransform.Rotator(), ActorSpawnParams);
		Multi_OnFire(ImpactResult);
		IAmmo--;
	}

	if (ImpactResult.Actor!=nullptr)
	{
		TSubclassOf<UDamageType> DamageType;
		UGameplayStatics::ApplyDamage(ImpactResult.GetActor(),10,GetOwner()->GetInstigatorController(),GetOwner(),DamageType);
	}
	OnWeaponFireDelegate.Broadcast();
}


void UTaskPlayerWeapon::OnEndFireWeapon()
{
	if (FireHandle.IsValid())
	{
		GetWorld()->GetTimerManager().ClearTimer(FireHandle);
		FireHandle.Invalidate();
	}
}



void UTaskPlayerWeapon::Multi_OnFire_Implementation(FHitResult ImpactResult)
{
	// try and play a firing animation if specified
	if (FireAnimation)
	{
		GetAnimInstance()->Montage_Play(FireAnimation, 1.f);
		if (OwnerPlayer && ReloadPlayerAnimation )
		{
			OwnerPlayer->GetMesh()->GetAnimInstance()->Montage_Play(FirePlayerAnimation);
			OwnerPlayer->GetMesh1P()->GetAnimInstance()->Montage_Play(FirePlayerAnimation);
		}
	}

	if (ImpactResult.bBlockingHit)
	{
		ACTFTaskCharacter*EnemyCharacter = Cast<ACTFTaskCharacter>(ImpactResult.GetActor());
		if(EnemyCharacter)
		{
			if (ImpactBodyParticle)
			{
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(),ImpactBodyParticle,ImpactResult.Location);
			}
			if (ImpactBodySound)
			{
				UGameplayStatics::SpawnSoundAtLocation(GetWorld(),ImpactBodySound,ImpactResult.Location);
			}
		}
		else
		{
			if (ImpactWallParticle)
			{
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(),ImpactWallParticle,ImpactResult.Location);
			}
			if (ImpactWallSound)
			{
				UGameplayStatics::SpawnSoundAtLocation(GetWorld(),ImpactWallSound,ImpactResult.Location);
			}
			
		}
	}
}






void UTaskPlayerWeapon::OnReloadWeapon()
{
	if (IAmmo<IAmmoMax)
	{
		GetWorld()->GetTimerManager().ClearTimer(FireHandle);
		FireHandle.Invalidate();
		Server_ReloadWeapon();
	}

}


void UTaskPlayerWeapon::Server_ReloadWeapon_Implementation()
{
	bisReloading=true;
	Multi_ReloadWeapon();
}


void UTaskPlayerWeapon::Multi_ReloadWeapon_Implementation()
{
	if (ReloadAnimation)	GetAnimInstance()->Montage_Play(ReloadAnimation);

	if (OwnerPlayer && ReloadPlayerAnimation )
	{
		OwnerPlayer->GetMesh()->GetAnimInstance()->Montage_Play(ReloadPlayerAnimation);
		OwnerPlayer->GetMesh1P()->GetAnimInstance()->Montage_Play(ReloadPlayerAnimation);
		GetWorld()->GetTimerManager().SetTimer(ReloadHandle,this,&UTaskPlayerWeapon::ReloadComplete,ReloadAnimation->GetPlayLength()-0.07,false);
	}
	
}


void UTaskPlayerWeapon::ReloadComplete()
{
	bisReloading=false;
	IAmmo=IAmmoMax;
	OnWeaponFireDelegate.Broadcast();
}





void UTaskPlayerWeapon::BeginPlay()
{
	Super::BeginPlay();

	OwnerPlayer=Cast<ACTFTaskCharacter>(GetOwner());

	OnWeaponFireDelegate.Broadcast();
}

void UTaskPlayerWeapon::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UTaskPlayerWeapon,OwnerPlayer);
	DOREPLIFETIME(UTaskPlayerWeapon,IAmmo);
	DOREPLIFETIME(UTaskPlayerWeapon,bisReloading);
}
