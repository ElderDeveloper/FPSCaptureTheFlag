#pragma once

#include "GameFramework/PlayerController.h"
#include "SGameplayData.h"
#include "TaskPlayerControllerLobby.generated.h"

class USLobbyWidget;

UCLASS()
class ATaskPlayerControllerLobby : public APlayerController
{
	GENERATED_BODY()
public:
	
	ATaskPlayerControllerLobby();
	
	UPROPERTY()
	USLobbyWidget*LobbyWidget;

	TSubclassOf<USLobbyWidget> LobbyWidgetClass;
	
	UFUNCTION(Client,Reliable,BlueprintCallable)
	void Client_InitialSetup();

	
	UFUNCTION(Client,Reliable)
	void Client_CreateLobbyWidget();


	UFUNCTION(Client,Reliable)
	void Client_ReceiveAllPlayerInformation(const TArray<FPlayersStruct>& PlayerArray);

	UFUNCTION(Client,Reliable)
	void Client_ReceivePlayerInformation(FPlayersStruct PlayerInfo);

	UFUNCTION(Client,Reliable)
	void Client_RemoveAllLobbyChilds();
	
	UFUNCTION(Client,Reliable)
	void Client_OnGameplayStarted();
	
	UFUNCTION(Client,Reliable)
	void Client_LeaveLobby();

	UFUNCTION(Client,Unreliable)
	void Client_ReceiveCountdown(int32 Countdown);

	
	UFUNCTION(Server,Reliable)
	void Server_RemovePlayerFromGameMode();
	
	UFUNCTION(Server,Reliable)
	void Server_CallUpdateAllPlayers(FName PlayerName,bool PlayerReady);

	
	UFUNCTION(Server,Reliable)
	void Server_AddPlayerToGameMode();


	void OnDestroySessionComplete (const FName SessionName, const bool Success);
	
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
};
