// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TaskGameMode.h"
#include "SGameplayData.h"




#include "TaskGameModeGameplay.generated.h"

/**
 * 
 */
class ATaskPlayerControllerGameplay;
class ATaskGameStateGameplay;
class ATaskTeamPlayerStart;
class ACTFTaskCharacter;

UCLASS()
class CTFTASK_API ATaskGameModeGameplay : public AGameModeBase
{
	GENERATED_BODY()

public:
	ATaskGameModeGameplay();


	UPROPERTY(Replicated)
	TArray<ATaskTeamPlayerStart*> PlayerStarts;

	
	UPROPERTY(EditDefaultsOnly,Category="Player")
	TSubclassOf<ACharacter> PlayerGameplayClass;

	
	void ReceiveTeamSelect(ATaskPlayerControllerGameplay * PlayerController);

	
	void ReceiveSpawnPlayer(ATaskPlayerControllerGameplay * PlayerController , TEnumAsByte<EPlayerTeam> Team);


	bool FindPlayerSpawnPoints(TEnumAsByte<EPlayerTeam> Team,FTransform& Transform);

	
	UPROPERTY()
	ATaskGameStateGameplay*	GPGameState;

	
	virtual void BeginPlay() override;

	virtual void GetLifetimeReplicatedProps(TArray < FLifetimeProperty > & OutLifetimeProps) const override;

};
