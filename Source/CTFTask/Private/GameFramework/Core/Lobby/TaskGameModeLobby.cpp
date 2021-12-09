#include "GameFramework/Core/Lobby/TaskGameModeLobby.h"

#include "GeneratedCodeHelpers.h"
#include "GameFramework/Core/Lobby/TaskPlayerControllerLobby.h"



void ATaskGameModeLobby::Server_UpdateAllPlayers_Implementation(FName PlayerName, bool PlayerReady)
{
	IReadyPlayerCount=0;
	
	for(int32 index=0 ; index<Players.Num();index++)
	{
		if (Players.IsValidIndex(index))
		{
			
			if (Players[index].PlayerName == PlayerName)
			{
				Players[index].PlayerReady=PlayerReady;
			}
			
			
			if(Players[index].PlayerReady)
			{
				IReadyPlayerCount++;
			}
		}	
	}

	GetWorld()->GetTimerManager().ClearTimer(CountdownHandle);
	CountdownHandle.Invalidate();

	ICountdown=5;
	if(PlayerControllerArray.Num()==IReadyPlayerCount)
	{
		GetWorld()->GetTimerManager().SetTimer(CountdownHandle,this,&ATaskGameModeLobby::CountdownEvent,1,true);
	}

	for (ATaskPlayerControllerLobby* i : PlayerControllerArray)
	{
		i->Client_RemoveAllLobbyChilds();
		
		for (FPlayersStruct S : Players)
		{
			i->Client_ReceivePlayerInformation(S);
		}
		//i->Client_ReceiveAllPlayerInformation(Players);
	}

}


void ATaskGameModeLobby::Server_AddPlayerInfo_Implementation(ATaskPlayerControllerLobby* PC,FPlayersStruct PlayersStruct)
{
	if (PC)
	{
		PlayerControllerArray.AddUnique(PC);
	}

	int32 index = Players.Add(PlayersStruct);
	Multi_AddPlayerInfo(PlayersStruct,index);
}


void ATaskGameModeLobby::Multi_AddPlayerInfo_Implementation(FPlayersStruct PlayersStruct, int32 index)
{
	if (Players.IsValidIndex(index))
	{
		Players[index] = PlayersStruct;
	}
}

void ATaskGameModeLobby::CountdownEvent()
{
	ICountdown--;
	
	for (ATaskPlayerControllerLobby* i : PlayerControllerArray)
	{
		i->Client_ReceiveCountdown(ICountdown);
	}
	
	if(ICountdown<=0)
	{

		for (ATaskPlayerControllerLobby* i : PlayerControllerArray)
		{
			i->Client_OnGameplayStarted();
		}

		GetWorld()->GetTimerManager().ClearTimer(CountdownHandle);
		CountdownHandle.Invalidate();

		//bUseSeamlessTravel=true;
		GetWorld()->ServerTravel("Gameplay");
	}
}


void ATaskGameModeLobby::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(ATaskGameModeLobby,IReadyPlayerCount);
	DOREPLIFETIME(ATaskGameModeLobby,Players);
	DOREPLIFETIME(ATaskGameModeLobby,PlayerControllerArray);
	DOREPLIFETIME(ATaskGameModeLobby,ICountdown);
}
