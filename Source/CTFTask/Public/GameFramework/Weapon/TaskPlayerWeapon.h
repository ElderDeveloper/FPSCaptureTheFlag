#pragma once
#include "Components/SkeletalMeshComponent.h"
#include "TaskPlayerWeapon.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnWeaponFire);

class USoundBase;
class UAnimMontage;
class USceneComponent;

UCLASS()
class UTaskPlayerWeapon : public  USkeletalMeshComponent
{
	GENERATED_BODY()
public:

	UTaskPlayerWeapon();

	/** Location on gun mesh where projectiles should spawn. */
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	USceneComponent* FP_MuzzleLocation;

	
	/** Projectile class to spawn */
	UPROPERTY(EditDefaultsOnly, Category=Projectile)
	TSubclassOf<class ACTFTaskProjectile> ProjectileClass;


	
	/** AnimMontage to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	UAnimMontage* FireAnimation;

	
	/** Sound to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
	USoundBase* ReloadSound;

	
	/** AnimMontage to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	UAnimMontage* ReloadAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	int32 IAmmo=36;

	bool bisReloading;

	float FFireRate;

	FTimerHandle FireHandle;
	
	UFUNCTION(BlueprintCallable)
	virtual void OnStartFireWeapon();

	virtual void OnFire();

	UPROPERTY(BlueprintAssignable)
	FOnWeaponFire OnWeaponFireDelegate;
	
	UFUNCTION(BlueprintCallable)
	virtual void OnEndFireWeapon();
	
};
