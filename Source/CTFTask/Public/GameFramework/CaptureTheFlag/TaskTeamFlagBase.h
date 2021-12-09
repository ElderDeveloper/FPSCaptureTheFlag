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

	//Components
	UPROPERTY(VisibleDefaultsOnly,BlueprintReadWrite)
	USceneComponent* RootScene;
	
	UPROPERTY(VisibleDefaultsOnly,BlueprintReadWrite)
	USceneComponent* FlagSpawnPoint;

	UPROPERTY(VisibleDefaultsOnly,BlueprintReadWrite)
	UStaticMeshComponent*FlagAreaMesh;

	UPROPERTY(VisibleDefaultsOnly,BlueprintReadWrite)
	UCapsuleComponent*FlagCollision;


	// Flag Variables
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	TEnumAsByte<EPlayerTeam> FlagBaseTeam;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ATaskTeamFlag> TeamFlagClass;

	UPROPERTY(Replicated,BlueprintReadOnly)
	ATaskTeamFlag*TeamFlag;

	UPROPERTY(Replicated,BlueprintReadOnly)
	bool bFlagIsInBase;

	
	// Flag Events

	UFUNCTION(Server,Reliable)
	void Server_SpawnTeamFlag();

	UFUNCTION(NetMulticast,Reliable)
	void Multi_FlagSpawned(ATaskTeamFlag* Ref);
	
	void EnemyFlagCaptured();

	void ReturnFlagToBase();

	void TeamLostFlag();

	// Overrides
	UFUNCTION()
    void OnCapsuleOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	virtual void BeginPlay() override;
	
	virtual void GetLifetimeReplicatedProps(TArray < FLifetimeProperty > & OutLifetimeProps) const override;
	
	
};
