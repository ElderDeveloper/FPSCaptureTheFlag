#include "GameFramework/Core/Lobby/TaskPlayerControllerLobby.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Engine/TaskGameInstance.h"
#include "UI/SLobbyWidget.h"
#include "GameFramework/PlayerState.h"
#include "GameFramework/Core/Lobby/TaskGameModeLobby.h"
#include "Kismet/GameplayStatics.h"


ATaskPlayerControllerLobby::ATaskPlayerControllerLobby()
{
	const ConstructorHelpers::FClassFinder<UUserWidget> LobbyWidgetClassConstruct (TEXT("/Game/UI/W_Lobby"));
	if(!ensure(LobbyWidgetClassConstruct.Class!= nullptr)) return;
	LobbyWidgetClass=LobbyWidgetClassConstruct.Class;
}

void ATaskPlayerControllerLobby::Client_ReceivePlayerInformation_Implementation(FPlayersStruct PlayerInfo)
{
	
	if(LobbyWidget)
	{
		LobbyWidget->UpdatePlayersWindow(PlayerInfo.PlayerName,PlayerInfo.PlayerReady,PlayerInfo.PlayerName==FName(*PlayerState->GetPlayerName()));
	}
	
		
}

void ATaskPlayerControllerLobby::Client_RemoveAllLobbyChilds_Implementation()
{
	if (LobbyWidget)
	{
		LobbyWidget->ClearPlayerWindow();
	}
}


void ATaskPlayerControllerLobby::Client_InitialSetup_Implementation()
{
	if (PlayerState)
	{
		Server_CallUpdateAllPlayers(FName(*PlayerState->GetPlayerName()),false);
	}
}


void ATaskPlayerControllerLobby::Client_CreateLobbyWidget_Implementation()
{
	if(!LobbyWidget)
	{
		LobbyWidget=CreateWidget<USLobbyWidget>(this,LobbyWidgetClass);
		if(LobbyWidget)
		{
			LobbyWidget->AddToViewport(1);

			UWidgetBlueprintLibrary::SetInputMode_UIOnlyEx(this,LobbyWidget);
			bShowMouseCursor=true;
		}
	}

	
}



void ATaskPlayerControllerLobby::Client_ReceiveAllPlayerInformation_Implementation(const TArray<FPlayersStruct>& PlayerArray)
{
	
	if(LobbyWidget)
	{
		LobbyWidget->ClearPlayerWindow();
		
		for (FPlayersStruct i : PlayerArray)
		{
			LobbyWidget->UpdatePlayersWindow(i.PlayerName,i.PlayerReady,i.PlayerName==FName(*PlayerState->GetPlayerName()));
		}
		
	}
	

}



void ATaskPlayerControllerLobby::Client_OnGameplayStarted_Implementation()
{
	UWidgetBlueprintLibrary::SetInputMode_GameOnly(this);
	if(LobbyWidget)
	{
		LobbyWidget->RemoveFromParent();
	}
}



void ATaskPlayerControllerLobby::Client_LeaveLobby_Implementation()
{
	Server_RemovePlayerFromGameMode();

	if (LobbyWidget)
	{
		LobbyWidget->RemoveFromParent();
	}

	UTaskGameInstance*GI = Cast<UTaskGameInstance>(GetWorld()->GetGameInstance());
	if(GI)
	{
		GI->SessionInterface->OnDestroySessionCompleteDelegates.AddUObject(this,&ATaskPlayerControllerLobby::OnDestroySessionComplete);
		GI->SessionInterface->DestroySession(SESSION_NAME);
	}
	
}



void ATaskPlayerControllerLobby::Server_RemovePlayerFromGameMode_Implementation()
{
	ATaskGameModeLobby*GM=Cast<ATaskGameModeLobby>(GetWorld()->GetAuthGameMode());
	if(GM && PlayerState)
	{
		GM->PlayerControllerArray.Remove(this);

		for (int32 i =0; i<GM->Players.Num() ; i++)
		{
			if(GM->Players.IsValidIndex(i))
			{
				
				if(GM->Players[i].PlayerName == FName(*PlayerState->GetPlayerName()))
				{
					GM->Players.RemoveAt(i);
					break;
				}
				
			}
		}
		GM->Server_UpdateAllPlayers(FName(),false);
	}
}



void ATaskPlayerControllerLobby::Server_CallUpdateAllPlayers_Implementation(FName PlayerName, bool PlayerReady)
{
	ATaskGameModeLobby*GM=Cast<ATaskGameModeLobby>(GetWorld()->GetAuthGameMode());
	if(GM)
	{
		GM->Server_UpdateAllPlayers(PlayerName,PlayerReady);
	}
}



void ATaskPlayerControllerLobby::Server_AddPlayerToGameMode_Implementation()
{
	
	if (PlayerState)
	{
		if (PlayerState->GetPlayerName().Len()>0)
		{
			ATaskGameModeLobby*GM=Cast<ATaskGameModeLobby>(GetWorld()->GetAuthGameMode());
			FPlayersStruct PStruct(FName(*PlayerState->GetPlayerName()),false);
			GM->Server_AddPlayerInfo(this,PStruct);
		}
	}
	
}


void ATaskPlayerControllerLobby::Client_ReceiveCountdown_Implementation(int32 Countdown)
{
	if(LobbyWidget)
	{
		LobbyWidget->CountdownReceived(Countdown);
	}
}


void ATaskPlayerControllerLobby::OnDestroySessionComplete(const FName SessionName, const bool Success)
{
	UGameplayStatics::OpenLevel(GetWorld(),"MainMenu",true);
}


void ATaskPlayerControllerLobby::BeginPlay()
{
	Super::BeginPlay();

	Client_CreateLobbyWidget();

	Server_AddPlayerToGameMode();
	
}


void ATaskPlayerControllerLobby::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}
