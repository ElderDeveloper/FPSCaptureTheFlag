#pragma once

#include "GameFramework/GameModeBase.h"
#include "SGameplayData.h"
#include "TaskGameModeLobby.generated.h"




UCLASS()
class ATaskGameModeLobby : public AGameModeBase
{
	GENERATED_BODY()
public:

	UPROPERTY(BlueprintReadWrite,Replicated)
	TArray<FPlayersStruct> Players;

	UPROPERTY(BlueprintReadWrite,Replicated)
	TArray<class ATaskPlayerControllerLobby*> PlayerControllerArray;

	UPROPERTY(Replicated)
	int32 IReadyPlayerCount=0;

	UPROPERTY(BlueprintReadWrite,Replicated)
	int32 ICountdown=5;

	FTimerHandle CountdownHandle;
	
	UFUNCTION(Server,Reliable)
	void Server_UpdateAllPlayers(FName PlayerName,bool PlayerReady);


	UFUNCTION(Server,Reliable)
	void Server_AddPlayerInfo(ATaskPlayerControllerLobby* PC , FPlayersStruct PlayersStruct);

	UFUNCTION(NetMulticast,Reliable)
	void Multi_AddPlayerInfo(FPlayersStruct PlayersStruct ,int32 index);
	
	UFUNCTION()
	void CountdownEvent();

	virtual void GetLifetimeReplicatedProps(TArray < FLifetimeProperty > & OutLifetimeProps) const override;
};
