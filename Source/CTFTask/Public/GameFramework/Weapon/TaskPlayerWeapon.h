#pragma once
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/Player/TaskCharacter.h"

#include "TaskPlayerWeapon.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnWeaponFire);

class USoundBase;
class UAnimMontage;
class USceneComponent;
class UParticleSystem;
class USoundBase;
UCLASS()
class UTaskPlayerWeapon : public  USkeletalMeshComponent
{
	GENERATED_BODY()
public:

	UTaskPlayerWeapon();

	/** Projectile class to spawn */
	UPROPERTY(EditDefaultsOnly, Category="Weapon|Fire")
	TSubclassOf<class ACTFTaskProjectile> ProjectileClass;

	UPROPERTY(EditDefaultsOnly, Category="Weapon|Fire")
	FName FireSocketName="Muzzle";

	UPROPERTY(EditDefaultsOnly, Category="Weapon|Fire")
	float FFireRate=0.1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite,Replicated, Category = "Weapon|Fire")
	int32 IAmmo=24;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite,Replicated, Category = "Weapon|Fire")
	int32 IAmmoMax=24;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon|Fire")
	float SpreadAndRecoil=2;

	
	/** AnimMontage to play each time we fire */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon|Animation")
	UAnimMontage* FireAnimation;

	/** AnimMontage to play each time we fire */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon|Animation")
	UAnimMontage* FirePlayerAnimation;

	/** AnimMontage to play each time we fire */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon|Animation")
	UAnimMontage* ReloadAnimation;

	/** AnimMontage to play each time we fire */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon|Animation")
	UAnimMontage* ReloadPlayerAnimation;

	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon|Impact")
	UParticleSystem*ImpactWallParticle;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon|Impact")
	UParticleSystem*ImpactBodyParticle;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon|Impact")
	USoundBase*ImpactWallSound;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon|Impact")
	USoundBase*ImpactBodySound;

	

	UPROPERTY(Replicated,BlueprintReadOnly)
	bool bisReloading;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Gameplay)
	TEnumAsByte<ECollisionChannel> TraceChannel;
	
	UPROPERTY(Replicated,BlueprintReadOnly)
	ACTFTaskCharacter*OwnerPlayer;


	FTimerHandle FireHandle;

	FTimerHandle ReloadHandle;

	
	UPROPERTY(BlueprintAssignable)
	FOnWeaponFire OnWeaponFireDelegate;

	
	UFUNCTION(BlueprintCallable)
	virtual void OnStartFireWeapon();

	UFUNCTION(BlueprintCallable)
	virtual void OnEndFireWeapon();

	void OnReloadWeapon();
	


protected:
	
	virtual void BeginPlay() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:
	
	virtual void OnFire();

	UFUNCTION(Server,Reliable)
    virtual void Server_OnFire(const FTransform SpawnTransform,FHitResult ImpactResult);

	UFUNCTION(NetMulticast,Unreliable)
    virtual void Multi_OnFire(FHitResult ImpactResult);

	UFUNCTION(Server,Reliable)
    virtual void Server_ReloadWeapon();

	UFUNCTION(NetMulticast,Reliable)
    virtual void Multi_ReloadWeapon();
	
	void ReloadComplete();
	
};
