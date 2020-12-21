#pragma once

#include "SGameplayData.h"

#include "TaskTeamFlagBase.generated.h"

class UCapsuleComponent;
class USceneComponent;
class UStaticMeshComponent;
class ATaskTeamFlag;
UCLASS()
class ATaskTeamFlagBase : public AActor
{
	GENERATED_BODY()
public:

	ATaskTeamFlagBase();

	UPROPERTY(VisibleDefaultsOnly,BlueprintReadWrite)
	USceneComponent* RootScene;
	
	UPROPERTY(VisibleDefaultsOnly,BlueprintReadWrite)
	USceneComponent* FlagSpawnPoint;

	UPROPERTY(VisibleDefaultsOnly,BlueprintReadWrite)
	UStaticMeshComponent*FlagAreaMesh;

	UPROPERTY(VisibleDefaultsOnly,BlueprintReadWrite)
	UCapsuleComponent*FlagCollision;

	UPROPERTY(EditAnywhere)
	TEnumAsByte<EPlayerTeam> FlagBaseTeam;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ATaskTeamFlag> TeamFlagClass;

	UPROPERTY(Replicated)
	ATaskTeamFlag*TeamFlag;

	UPROPERTY(Replicated)
	bool bFlagIsInBase;

	UFUNCTION()
	void OnCapsuleOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);


	void EnemyFlagCaptured();

	void ReturnFlagToBase();

	void TeamLostFlag();

	virtual void BeginPlay() override;
	
	virtual void GetLifetimeReplicatedProps(TArray < FLifetimeProperty > & OutLifetimeProps) const override;
	
	
};
