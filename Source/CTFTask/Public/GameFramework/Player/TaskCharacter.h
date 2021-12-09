// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"




#include "GameFramework/Character.h"
#include "GameFramework/CaptureTheFlag/CaptureTheFlagInterface.h"
#include "GameFramework/CaptureTheFlag/TaskTeamFlag.h"

#include "TaskCharacter.generated.h"


enum EPlayerWidgetAction
{
	Ammo,
	Health,
	Death,
	Respawn,
	CaptureFlag,
	ScorePoint
};



class UInputComponent;
class USkeletalMeshComponent;
class USceneComponent;
class UCameraComponent;
class UTaskPlayerWeapon;

UCLASS(config=Game)
class ACTFTaskCharacter : public ACharacter , public ICaptureTheFlagInterface 
{
	GENERATED_BODY()
public:
	ACTFTaskCharacter();
	
	/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY(VisibleDefaultsOnly,BlueprintReadWrite, Category=Mesh)
	USkeletalMeshComponent* Mesh1P;

protected:
	UPROPERTY(VisibleDefaultsOnly, Category=Mesh)
	USkeletalMeshComponent* FakeWeaponMesh;

	UPROPERTY(VisibleDefaultsOnly,BlueprintReadWrite, Category = Mesh)
	UTaskPlayerWeapon * PlayerWeapon;

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FirstPersonCameraComponent;

	UPROPERTY(VisibleDefaultsOnly,BlueprintReadOnly, Category = Widget)
	class UWidgetComponent*PlayerNameWidget;

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Replicated,Category=Health)
	float FHealth = 100;

	
public:
	
	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

	/** AnimMontage to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	class UAnimMontage* FireAnimation;


protected:
	
	/** Fires a projectile. */
	void OnFire();

	void OnEndFire();

	UFUNCTION()
	void UpdateAmmoWidget();

	void OnReaload();
	
	//Handle Sprint Movement
	void ActivateSprint();

	void DeactivateSprint();

	void OnDeath();
	
	/** Handles moving forward/backward */
	void MoveForward(float Val);

	/** Handles stafing movement, left and right */
	void MoveRight(float Val);

	/**
	* Called via input to turn at a given rate.
	* @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	*/
	void TurnAtRate(float Rate);

	/**
	* Called via input to turn look up/down at a given rate.
	* @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	*/
	void LookUpAtRate(float Rate);


	UFUNCTION(Server,Unreliable)
	void Server_SetLookUpAlpha(float Value);

	UFUNCTION(Client,Unreliable)
	void Client_OnHealthUpdate();

	UFUNCTION(Client,Reliable)
	void Client_OnPlayerDeathUpdate(AController* KillerInstigator);

	UFUNCTION(Client,Reliable)
	void Client_RequestRespawn();

public:
	
	UPROPERTY(BlueprintReadOnly,Replicated)
	float FLookUpAlpha;

	UPROPERTY(Replicated,BlueprintReadOnly)
	ATaskTeamFlag* FlagReference = nullptr;

	UPROPERTY(Replicated,BlueprintReadOnly)
	TEnumAsByte<EPlayerTeam>PlayerTeam;

	UPROPERTY()
	class ATaskPlayerControllerGameplay*PlayerControllerGameplay;
	UPROPERTY()
	class ATaskPlayerStateGameplay*PlayerStateGameplay;
	UPROPERTY(Replicated)
	class ATaskGameStateGameplay*GameStateGameplay;

	virtual EPlayerTeam GetHasFlagAndTeam() override;

	virtual EPlayerTeam GetPlayerTeam() override;

	virtual ATaskTeamFlag* GetTeamFlag() override;

	virtual void ReceiveEnemyFlag(ATaskTeamFlag* FlagRef) override;

	virtual void ReceiveFlagCarriedToStation()	override;
	
	FORCEINLINE	bool GetHasCarryingFlag() const {return IsValid(FlagReference);}
	
protected:
	// APawn interface

	virtual void BeginPlay() override;
	
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void Tick(float DeltaSeconds) override;
	
public:
	/** Returns Mesh1P subobject **/
	FORCEINLINE class USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }
	/** Returns FirstPersonCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

};

