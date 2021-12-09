#pragma once
#include "GameFramework/PlayerStart.h"
#include "SGameplayData.h"
#include "TaskTeamPlayerStart.generated.h"

UCLASS()
class ATaskTeamPlayerStart : public  APlayerStart
{
	GENERATED_BODY()
public:


	
	UPROPERTY(EditAnywhere)
	TEnumAsByte<EPlayerTeam> PlayerStartTeam;


	UFUNCTION(Server,Reliable)
	void Server_SendReferanceToGameMode();

	
	virtual void BeginPlay() override;
};
