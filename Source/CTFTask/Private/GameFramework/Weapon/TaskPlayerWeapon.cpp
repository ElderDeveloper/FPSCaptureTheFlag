#include "GameFramework/Weapon/TaskPlayerWeapon.h"
#include "GameFramework/Weapon/TaskProjectile.h"
#include "Animation/AnimInstance.h"

UTaskPlayerWeapon::UTaskPlayerWeapon()
{
	
}

void UTaskPlayerWeapon::OnStartFireWeapon()
{
	if(ProjectileClass!=NULL && IAmmo!=0 && !bisReloading)
	{
		GetWorld()->GetTimerManager().SetTimer(FireHandle,this,&UTaskPlayerWeapon::OnFire,FFireRate,true);
		OnFire();
	}
	
}

void UTaskPlayerWeapon::OnFire()
{
	const FRotator SpawnRotation = GetComponentRotation();
	// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
	const FVector SpawnLocation = FP_MuzzleLocation->GetComponentLocation();

	//Set Spawn Collision Handling Override
	FActorSpawnParameters ActorSpawnParams;
	ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

	// spawn the projectile at the muzzle
	GetWorld()->SpawnActor<ACTFTaskProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);

	// try and play a firing animation if specified
	if (FireAnimation != nullptr)
	{
		// Get the animation object for the arms mesh
		UAnimInstance* AnimInstance = GetAnimInstance();
		if (AnimInstance != nullptr)
		{
			AnimInstance->Montage_Play(FireAnimation, 1.f);
		}
	}
	
	OnWeaponFireDelegate.Broadcast();

}

void UTaskPlayerWeapon::OnEndFireWeapon()
{
	if (FireHandle.IsValid())
	{
		FireHandle.Invalidate();
	}
}
