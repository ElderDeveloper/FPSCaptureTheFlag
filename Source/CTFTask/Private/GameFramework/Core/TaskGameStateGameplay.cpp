#include "GameFramework/Core/TaskGameStateGameplay.h"
#include "GameFramework/Player/TaskPlayerControllerGameplay.h"
#include "GameFramework/Player/TaskPlayerStateGameplay.h"
#include "GeneratedCodeHelpers.h"

ATaskGameStateGameplay::ATaskGameStateGameplay()
{
	
}

void ATaskGameStateGameplay::Server_AddNewPlayerToTheTeam(TEnumAsByte<EPlayerTeam> PlayerT,ATaskPlayerControllerGameplay* PlayerC)
{
	switch (PlayerT)
	{
	case None:
		GEngine->AddOnScreenDebugMessage(-1,6.f,FColor::Red,"Team Received is Empty");
	break;
		
	case BlueTeam:
		IBlueTeamPlayerCount+=1;
		AddPlayerControllerToTeam(BlueTeam,PlayerC);
	break;

	case RedTeam:
		IRedTeamPlayerCount+=1;
		AddPlayerControllerToTeam(RedTeam,PlayerC);
		break;
	default:
	break;
	}
	
}


void ATaskGameStateGameplay::StartGameTimeCountdown()
{
	if (!GameTimeDecreaseHandle.IsValid())
	{
		GetWorld()->GetTimerManager().SetTimer(GameTimeDecreaseHandle,this,&ATaskGameStateGameplay::DecreaseGameTime,FTimeDecreaseInterval,true);
	}
}

void ATaskGameStateGameplay::AddScoreToTheTeam(TEnumAsByte<EPlayerTeam> Team)
{
	if (Team!=None)
	{
		if (Team==BlueTeam)
		{
			IBlueTeamScore+=1;
			OnTeamScoreChanged.Broadcast(IBlueTeamScore,IRedTeamScore);
			return;
		}

		IRedTeamScore+=1;
		OnTeamScoreChanged.Broadcast(IBlueTeamScore,IRedTeamScore);
	}
}

void ATaskGameStateGameplay::DecreaseGameTime()
{
	FGameTimeLeft=FGameTimeLeft-FTimeDecreaseInterval;
	
	OnGameTimeChanged.Broadcast(FGameTimeLeft);
	
	if(FGameTimeLeft<=0)
	{
		GetWorld()->GetTimerManager().ClearTimer(GameTimeDecreaseHandle);
		GameTimeDecreaseHandle.Invalidate();
		
		if(IBlueTeamScore>IRedTeamScore)
		{
			WinnerTeam=BlueTeam;
		}
		else if (IRedTeamScore>IBlueTeamScore)
		{
			WinnerTeam=RedTeam;
		}

		OnEndGame.Broadcast(WinnerTeam);
		
	}
}

void ATaskGameStateGameplay::OnReceiveKill(ATaskPlayerStateGameplay* KillerPlayerState,ATaskPlayerStateGameplay* VictimPlayerState)
{
	if(KillerPlayerState)
	{
		if (KillerPlayerState->PlayerTeam!=None)
		{
			if (KillerPlayerState->PlayerTeam==BlueTeam)
			{
				IBlueTeamKillScore+=1;
				OnPlayerKill.Broadcast(KillerPlayerState,VictimPlayerState);
				return;
			}

			IRedTeamKillScore+=1;
			OnPlayerKill.Broadcast(KillerPlayerState,VictimPlayerState);
			return;
		}

	}
}


void ATaskGameStateGameplay::AddPlayerControllerToTeam_Implementation(EPlayerTeam Team , ATaskPlayerControllerGameplay* NewPlayer)
{
	switch (Team)
	{
		case BlueTeam:		BlueTeamPlayers.Add(NewPlayer);	break;

		case RedTeam:		RedTeamPlayers.Add(NewPlayer);	break;
		
		default:	break;
	}

}

void ATaskGameStateGameplay::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ATaskGameStateGameplay,IBlueTeamPlayerCount);
	DOREPLIFETIME(ATaskGameStateGameplay,IRedTeamPlayerCount);
	DOREPLIFETIME(ATaskGameStateGameplay,BlueTeamPlayers);
	DOREPLIFETIME(ATaskGameStateGameplay,RedTeamPlayers);
	DOREPLIFETIME(ATaskGameStateGameplay,IBlueTeamScore);
	DOREPLIFETIME(ATaskGameStateGameplay,IRedTeamScore);
	DOREPLIFETIME(ATaskGameStateGameplay,IBlueTeamKillScore);
	DOREPLIFETIME(ATaskGameStateGameplay,IRedTeamKillScore);
	DOREPLIFETIME(ATaskGameStateGameplay,FGameTimeLeft);
}
