// Copyright Epic Games, Inc. All Rights Reserved.

#include "GameFramework/Player/TaskCharacter.h"
#include "GameFramework/PlayerState.h"
#include "GeneratedCodeHelpers.h"
#include "GameFramework/Weapon/TaskProjectile.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/InputSettings.h"
#include "Kismet/GameplayStatics.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/Core/TaskGameStateGameplay.h"
#include "GameFramework/Player/TaskPlayerControllerGameplay.h"
#include "GameFramework/Player/TaskPlayerStateGameplay.h"
#include "GameFramework/Weapon/TaskPlayerWeapon.h"

DEFINE_LOG_CATEGORY_STATIC(LogFPChar, Warning, All);

//////////////////////////////////////////////////////////////////////////
// ACTFTaskCharacter

ACTFTaskCharacter::ACTFTaskCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-39.56f, 1.75f, 64.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	Mesh1P->SetRelativeRotation(FRotator(1.9f, -19.19f, 5.2f));
	Mesh1P->SetRelativeLocation(FVector(-0.5f, -4.4f, -155.7f));
	
	FakeWeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FakeWeaponMesh"));
	FakeWeaponMesh->SetOwnerNoSee(true);
	FakeWeaponMesh->bCastDynamicShadow = true;
	FakeWeaponMesh->CastShadow = true;
	FakeWeaponMesh->SetupAttachment(GetMesh(),"hand_r");
	
	// Create a gun mesh component
	PlayerWeapon = CreateDefaultSubobject<UTaskPlayerWeapon>(TEXT("PlayerWeapon"));
	PlayerWeapon->bCastDynamicShadow = false;
	PlayerWeapon->CastShadow = false;
	PlayerWeapon->SetOnlyOwnerSee(true);
	PlayerWeapon->SetupAttachment(Mesh1P,"hand_r");

	PlayerNameWidget=CreateDefaultSubobject<UWidgetComponent>(TEXT("PlayerNameWidget"));
	PlayerNameWidget->SetupAttachment(RootComponent);
	PlayerNameWidget->SetWidgetSpace(EWidgetSpace::Screen);
	PlayerNameWidget->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	PlayerNameWidget->CastShadow=false;
	
}



void ACTFTaskCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();
	Mesh1P->SetHiddenInGame(false, true);

	PlayerControllerGameplay=Cast<ATaskPlayerControllerGameplay>(GetController());
	
	if (PlayerControllerGameplay)
	{
		PlayerStateGameplay = Cast<ATaskPlayerStateGameplay>(PlayerControllerGameplay->PlayerState);
	}

	GameStateGameplay=Cast<ATaskGameStateGameplay>(GetWorld()->GetGameState());
	
	PlayerWeapon->OnWeaponFireDelegate.AddDynamic(this,&ACTFTaskCharacter::UpdateAmmoWidget);

	
	
}

//////////////////////////////////////////////////////////////////////////
// Input

EPlayerTeam ACTFTaskCharacter::GetHasFlagAndTeam()
{
	if (FlagReference)
	{
		return FlagReference->FlagTeam;
	}
	return None;
}



EPlayerTeam ACTFTaskCharacter::GetPlayerTeam()
{
	return PlayerTeam;
}



ATaskTeamFlag* ACTFTaskCharacter::GetTeamFlag()
{
	return FlagReference;
}



void ACTFTaskCharacter::ReceiveEnemyFlag(ATaskTeamFlag* FlagRef)
{
	FlagReference=FlagRef;
	
	if(FlagReference)
	{
		FAttachmentTransformRules Roules(EAttachmentRule::SnapToTarget,true);
		FlagReference->AttachToComponent(GetMesh(),Roules,"Flag");
	}

	if (PlayerControllerGameplay)
	{
		PlayerControllerGameplay->OnCaptureFlagStateChanged.Broadcast(true);
	}
}



void ACTFTaskCharacter::ReceiveFlagCarriedToStation()
{
	if(FlagReference)
	{
		const FDetachmentTransformRules Rules(EDetachmentRule::KeepWorld,false);
		FlagReference->DetachFromActor(Rules);
		FlagReference=nullptr;
		if (PlayerControllerGameplay)
		{
			PlayerControllerGameplay->OnCaptureFlagStateChanged.Broadcast(false);
		}
	}
}



void ACTFTaskCharacter::OnFire()
{
	if(PlayerWeapon)
	{
		PlayerWeapon->OnStartFireWeapon();
	}
}



void ACTFTaskCharacter::OnEndFire()
{
	if(PlayerWeapon)
	{
		PlayerWeapon->OnEndFireWeapon();
	}
	
}

void ACTFTaskCharacter::UpdateAmmoWidget()
{
	if(PlayerControllerGameplay)
	{
		PlayerControllerGameplay->OnAmmoChanged.Broadcast(PlayerWeapon->IAmmo);
	}
}


void ACTFTaskCharacter::OnReaload()
{
	if(PlayerWeapon)
	{
		PlayerWeapon->OnReloadWeapon();
	}
}



void ACTFTaskCharacter::ActivateSprint()
{
	
}



void ACTFTaskCharacter::DeactivateSprint()
{
	
}



void ACTFTaskCharacter::OnDeath()
{
	if (FlagReference)
	{
		FDetachmentTransformRules Roules(EDetachmentRule::KeepWorld,true);
		FlagReference->DetachFromActor(Roules);
		FlagReference->OnFlagDrop();
	}
	Client_RequestRespawn();
	Destroy();

}


void ACTFTaskCharacter::MoveForward(float Value)
{
	AddMovementInput(GetActorForwardVector(), Value);
}
void ACTFTaskCharacter::MoveRight(float Value)
{
	AddMovementInput(GetActorRightVector(), Value);
}
void ACTFTaskCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}



void ACTFTaskCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
	const float ControlRot = GetControlRotation().Pitch;

	Server_SetLookUpAlpha(ControlRot <= 90 ?
	UKismetMathLibrary::MapRangeClamped(ControlRot,0,90,0,1) :
	UKismetMathLibrary::MapRangeClamped(ControlRot,360,270,0,-1)); 
	
}


void ACTFTaskCharacter::Client_RequestRespawn_Implementation()
{
	if(PlayerControllerGameplay)
	{
		PlayerControllerGameplay->RequestRespawnPlayer();
	}
}


void ACTFTaskCharacter::Client_OnPlayerDeathUpdate_Implementation(AController* KillerInstigator)
{	
	if(KillerInstigator)
	{
		ATaskPlayerStateGameplay* PS = Cast<ATaskPlayerStateGameplay>(KillerInstigator->PlayerState);
		
		if (PlayerControllerGameplay && PS)
		{
			const FName KillerName = *PS->GetPlayerName();
			PlayerControllerGameplay->OnPlayerDeath.Broadcast(KillerName,PlayerTeam==BlueTeam ? RedTeam : BlueTeam);
		}
		else
		{
			if (PlayerControllerGameplay)
			{
				PlayerControllerGameplay->OnPlayerDeath.Broadcast("Player",PlayerTeam==BlueTeam ? RedTeam : BlueTeam);
			}
		}
	}
	else
	{	
		if (PlayerControllerGameplay)
		{
			PlayerControllerGameplay->OnPlayerDeath.Broadcast("Player",PlayerTeam==BlueTeam ? RedTeam : BlueTeam);
		}
	}

}


void ACTFTaskCharacter::Client_OnHealthUpdate_Implementation()
{
	if (PlayerControllerGameplay)
	{
		PlayerControllerGameplay->OnHealthChanged.Broadcast(FHealth/100);
	}
}


void ACTFTaskCharacter::Server_SetLookUpAlpha_Implementation(float Value)
{
	FLookUpAlpha=Value;
}



float ACTFTaskCharacter::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator,AActor* DamageCauser)
{
	FHealth=FHealth-Damage;
	
	Client_OnHealthUpdate();
	
	bool DelegateSend=false;
	
	if (FHealth<=0)
	{
		Client_OnPlayerDeathUpdate(EventInstigator);

		ATaskPlayerControllerGameplay* KillerController = Cast<ATaskPlayerControllerGameplay>(EventInstigator);
		if (KillerController && GameStateGameplay)
		{
			ATaskPlayerStateGameplay* KillerState = Cast<ATaskPlayerStateGameplay>(KillerController->PlayerState);
			if (KillerState)
			{
				GameStateGameplay->OnPlayerKill.Broadcast(KillerState,PlayerStateGameplay);
				DelegateSend=true;
			}
		}
		
		if (DelegateSend)
		{
			if (GameStateGameplay)
			{
				GameStateGameplay->OnPlayerKill.Broadcast(nullptr,PlayerStateGameplay);
			}
		}
		
		OnDeath();	
	}

	return Damage;
}



void ACTFTaskCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// set up gameplay key bindings
	check(PlayerInputComponent);

	// Bind jump events
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	// Bind fire event
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &ACTFTaskCharacter::OnFire);
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &ACTFTaskCharacter::OnEndFire);

	PlayerInputComponent->BindAction("Reload", IE_Pressed, this, &ACTFTaskCharacter::OnReaload);

	// Sprint Movement Events
	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &ACTFTaskCharacter::ActivateSprint);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &ACTFTaskCharacter::DeactivateSprint);

	// Bind movement events
	PlayerInputComponent->BindAxis("MoveForward", this, &ACTFTaskCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ACTFTaskCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &ACTFTaskCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &ACTFTaskCharacter::LookUpAtRate);
}



void ACTFTaskCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ACTFTaskCharacter,FLookUpAlpha);

	DOREPLIFETIME(ACTFTaskCharacter,PlayerTeam);

	DOREPLIFETIME(ACTFTaskCharacter,FlagReference);

	DOREPLIFETIME(ACTFTaskCharacter,FHealth);

	DOREPLIFETIME(ACTFTaskCharacter,GameStateGameplay);
}

void ACTFTaskCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}
