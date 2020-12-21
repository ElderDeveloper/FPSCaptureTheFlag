#include "GameFramework/Player/TaskPlayerControllerGameplay.h"


#include "GeneratedCodeHelpers.h"
#include "GameFramework/Core/TaskGameModeGameplay.h"
#include "GameFramework/Core/TaskGameStateGameplay.h"
#include "GameFramework/Player/TaskPlayerStateGameplay.h"
#include "UI/SGameplayWidget.h"



ATaskPlayerControllerGameplay::ATaskPlayerControllerGameplay(const FObjectInitializer& ObjectInitializer)
{
}

void ATaskPlayerControllerGameplay::SendOnAmmoChanged(int32 IAmmo)
{
	OnAmmoChanged.Broadcast(IAmmo);
}

void ATaskPlayerControllerGameplay::SendOnHealthChanged(float IHealth, float IMaxHealth)
{
	OnHealthChanged.Broadcast(IHealth/IMaxHealth);
}

void ATaskPlayerControllerGameplay::SendOnCaptureFlagStateChanged(bool bHasFlag)
{
	OnCaptureFlagStateChanged.Broadcast(bHasFlag);
}

void ATaskPlayerControllerGameplay::SendOnPlayerDeath(ATaskPlayerStateGameplay* KillerPState)
{
	if (KillerPState)
	{
		OnPlayerDeath.Broadcast(KillerPState);
	}

}


void ATaskPlayerControllerGameplay::CreateGameplayWidget()
{
	if(!WGameplayWidget)
	{
		WGameplayWidget=CreateWidget<USGameplayWidget>(this,WidgetClass);
		if(WGameplayWidget)
		{
			WGameplayWidget->AddToViewport(1);
			GEngine->AddOnScreenDebugMessage(-1,5.f,FColor::Green,"Create Success");
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1,5.f,FColor::Red,"Create Failed");
		}

	}
}


void ATaskPlayerControllerGameplay::RequestTeamSelect()
{

	if (GetLocalRole()==ROLE_Authority)
	{
		Server_RequestTeamSelect();
	}
}

void ATaskPlayerControllerGameplay::Server_RequestTeamSelect_Implementation()
{
	ATaskGameModeGameplay*GM=Cast<ATaskGameModeGameplay>(GetWorld()->GetAuthGameMode());
	
	if (GM!=nullptr)
	{
		GM->ReceiveTeamSelect(this);
	}
}


void ATaskPlayerControllerGameplay::TeamSelectReceived(TEnumAsByte<EPlayerTeam> ReceivedTeam)
{
	ATaskGameModeGameplay*GM=Cast<ATaskGameModeGameplay>(GetWorld()->GetAuthGameMode());
	if (GM!=nullptr)
	{
		GM->ReceiveSpawnPlayer(this,ReceivedTeam);
	}

	PlayerTeam=ReceivedTeam;

	ATaskPlayerStateGameplay* PS=Cast<ATaskPlayerStateGameplay>(GetPlayerState<ATaskPlayerStateGameplay>());
	if(PlayerState!=nullptr)
	{
		PS->PlayerTeam=ReceivedTeam;
	}

	ATaskGameStateGameplay* GameState = Cast<ATaskGameStateGameplay>(GetWorld()->GetGameState());
	if (GameState!=nullptr)
	{
		GameState->Server_AddNewPlayerToTheTeam(ReceivedTeam,this);
	}
	
	
}


void ATaskPlayerControllerGameplay::RequestRespawnPlayer()
{
	GetWorld()->GetTimerManager().SetTimer(RespawnTimeHandle,this,&ATaskPlayerControllerGameplay::Server_RequestRespawnPlayer,RespawnTime,false);
}

void ATaskPlayerControllerGameplay::Server_RequestRespawnPlayer_Implementation()
{
	ATaskGameModeGameplay*GM=Cast<ATaskGameModeGameplay>(GetWorld()->GetAuthGameMode());
	if (GM)
	{
		GM->ReceiveSpawnPlayer(this,PlayerTeam);
	}
}


void ATaskPlayerControllerGameplay::BeginPlay()
{
	Super::BeginPlay();
	
	RequestTeamSelect();
}

void ATaskPlayerControllerGameplay::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ATaskPlayerControllerGameplay,PlayerTeam);
}
