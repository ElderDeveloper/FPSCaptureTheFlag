#pragma once

#include "SGameplayData.h"

#include "TaskTeamFlag.generated.h"

class UCapsuleComponent;
class UStaticMeshComponent;
class USceneComponent;
class UMaterialInstance;
UCLASS()
class ATaskTeamFlag : public AActor
{
	GENERATED_BODY()
public:
	ATaskTeamFlag();

	UPROPERTY(VisibleDefaultsOnly,BlueprintReadWrite)
	USceneComponent* RootScene;
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite)
	UStaticMeshComponent*FlagMesh;

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite)
	UCapsuleComponent*FlagCollision;


	UPROPERTY(Replicated)
	TEnumAsByte<EPlayerTeam> FlagTeam;

	
	UPROPERTY(EditDefaultsOnly)
	UMaterialInstance* BlueFlagMaterial;

	UPROPERTY(EditDefaultsOnly)
	UMaterialInstance* RedFlagMaterial;

	UPROPERTY(Replicated)
	class ATaskTeamFlagBase*FlagBase;
	
	void SetupFlagTeam(TEnumAsByte<EPlayerTeam> Team, ATaskTeamFlagBase*Base);

	virtual void OnFlagDrop();
	
	virtual void OnFlagTakenFromGround();

	virtual void OnFlagReturnToBase();

	UFUNCTION()
    void OnCapsuleOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	virtual void BeginPlay() override;
	
	virtual void GetLifetimeReplicatedProps(TArray < FLifetimeProperty > & OutLifetimeProps) const override;
};
