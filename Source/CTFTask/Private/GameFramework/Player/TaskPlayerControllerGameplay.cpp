#include "GameFramework/Player/TaskPlayerControllerGameplay.h"


#include "GeneratedCodeHelpers.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "GameFramework/Core/TaskGameModeGameplay.h"
#include "GameFramework/Core/TaskGameStateGameplay.h"
#include "GameFramework/Player/TaskPlayerStateGameplay.h"
#include "UI/SEndGameWidget.h"
#include "UI/SGameplayWidget.h"
#include "UI/SInGamePauseMenuWidget.h"
#include "UI/SScoreBoardWidget.h"


ATaskPlayerControllerGameplay::ATaskPlayerControllerGameplay(const FObjectInitializer& ObjectInitializer)
{
	const ConstructorHelpers::FClassFinder<UUserWidget> GameplayMenuClassConstruct (TEXT("/Game/UI/W_GameplayWidget"));
	if(!ensure(GameplayMenuClassConstruct.Class!= nullptr)) return;
	GameplayWidgetClass=GameplayMenuClassConstruct.Class;

	
	const ConstructorHelpers::FClassFinder<UUserWidget> ScoreBoardClassConstruct (TEXT("/Game/UI/W_ScoreBoard"));
	if(!ensure(ScoreBoardClassConstruct.Class!= nullptr)) return;
	ScoreBoardClass=ScoreBoardClassConstruct.Class;

	
	const ConstructorHelpers::FClassFinder<UUserWidget> InGamePauseClassConstruct (TEXT("/Game/UI/W_InGamePauseMenu"));
	if(!ensure(InGamePauseClassConstruct.Class!= nullptr)) return;
	InGamePauseClass=InGamePauseClassConstruct.Class;


	const ConstructorHelpers::FClassFinder<UUserWidget> KillNotifyClassConstruct (TEXT("/Game/UI/W_InGamePauseMenu"));
	if(!ensure(KillNotifyClassConstruct.Class!= nullptr)) return;
	KillNotifyClass=KillNotifyClassConstruct.Class;

	const ConstructorHelpers::FClassFinder<UUserWidget> EndGameWidgetConstruct (TEXT("/Game/UI/W_EndGameMenu"));
	if(!ensure(EndGameWidgetConstruct.Class!= nullptr)) return;
	EndGameWidgetClass=EndGameWidgetConstruct.Class;

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



void ATaskPlayerControllerGameplay::CreateGameplayWidget()
{

	if(!WGameplayWidget)
	{
		WGameplayWidget=CreateWidget<USGameplayWidget>(this,GameplayWidgetClass);
		if(WGameplayWidget)
		{
			WGameplayWidget->AddToViewport(1);
		}
	}
}



void ATaskPlayerControllerGameplay::CreateDestroyScoreWidget()
{
	if (!WInGamePauseWidget)
	{
		if (WScoreBoardWidget)
		{
			WScoreBoardWidget->RemoveFromParent();
			WScoreBoardWidget=nullptr;
		}
		else
		{
			WScoreBoardWidget=CreateWidget<USScoreBoardWidget>(this, ScoreBoardClass);
			if(WScoreBoardWidget)
			{
				WScoreBoardWidget->AddToViewport(2);
			}
		}
	}
}




void ATaskPlayerControllerGameplay::CreateDestroyInGamePauseWidget()
{
	if (!WScoreBoardWidget)
	{
		WInGamePauseWidget=CreateWidget<USInGamePauseMenuWidget>(this, InGamePauseClass);
		if(WInGamePauseWidget)
		{
			WInGamePauseWidget->AddToViewport(2);
			WInGamePauseWidget->OnDeconstruct.AddDynamic(this,&ATaskPlayerControllerGameplay::InGamePauseMenuDeconstruct);
		}
	}
}



void ATaskPlayerControllerGameplay::InGamePauseMenuDeconstruct()
{
	if (WInGamePauseWidget)
	{
		WInGamePauseWidget=nullptr;
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

void ATaskPlayerControllerGameplay::EndGameWidgetTrigger(TEnumAsByte<EPlayerTeam> WinnerTeam)
{
	Client_ShowEndGameWidget(WinnerTeam);
}

void ATaskPlayerControllerGameplay::Client_ShowEndGameWidget_Implementation(uint8 WinnerTeam)
{
	
	if (EndGameWidgetClass)
	{
		WEndGameWidget=CreateWidget<USEndGameWidget>(this,EndGameWidgetClass);
		
		if(WEndGameWidget)
		{
			TEnumAsByte<EPlayerTeam> WT;
			switch (WinnerTeam)
			{
				case 0:WT=None;
					break;
				case 1:WT=BlueTeam;
					break;
				case 2:WT=RedTeam;
					break;
				default: break;
			}
			WEndGameWidget->AddToViewport(4);
			WEndGameWidget->SetupWidget(WT);
			
			UWidgetBlueprintLibrary::SetInputMode_UIOnlyEx(this,WEndGameWidget);
			bShowMouseCursor=true;
		}
	}
	
	if (WGameplayWidget)
	{
		WGameplayWidget->RemoveFromParent();
	}
	if(GetPawn())
	{
		GetPawn()->Destroy();
	}
	GetWorld()->GetTimerManager().ClearTimer(RespawnTimeHandle);

}





void ATaskPlayerControllerGameplay::RequestRespawnPlayer()
{
	FCurrentRespawnCountdown=FRespawnCountdown;
	OnPlayerRespawnCountdown.Broadcast(FCurrentRespawnCountdown);
	GetWorld()->GetTimerManager().SetTimer(RespawnTimeHandle,this,&ATaskPlayerControllerGameplay::RespawnCountdown,1,true);
}

void ATaskPlayerControllerGameplay::RespawnCountdown()
{
	FCurrentRespawnCountdown-=1;
	OnPlayerRespawnCountdown.Broadcast(FCurrentRespawnCountdown);
	if (FCurrentRespawnCountdown<=0)
	{
		GetWorld()->GetTimerManager().ClearTimer(RespawnTimeHandle);
		OnPlayerRespawn.Broadcast();
		Server_RequestRespawnPlayer();
	}
}


void ATaskPlayerControllerGameplay::Server_RequestRespawnPlayer_Implementation()
{
	ATaskGameModeGameplay*GM=Cast<ATaskGameModeGameplay>(GetWorld()->GetAuthGameMode());
	if (GM)
	{
		GM->ReceiveSpawnPlayer(this,PlayerTeam);
	}
}



ATaskPlayerStateGameplay* ATaskPlayerControllerGameplay::ReturnPlayerStateAsGameplay()
{
	return GetPlayerState<ATaskPlayerStateGameplay>();
}



void ATaskPlayerControllerGameplay::BeginPlay()
{
	Super::BeginPlay();
	
	RequestTeamSelect();

	ATaskGameStateGameplay*GS = Cast<ATaskGameStateGameplay>(GetWorld()->GetGameState());
	if(GS)
	{
		GS->OnEndGame.AddDynamic(this,&ATaskPlayerControllerGameplay::EndGameWidgetTrigger);
	}
}


void ATaskPlayerControllerGameplay::SetupInputComponent()
{
	Super::SetupInputComponent();
	check(InputComponent);
	
	InputComponent->BindAction("ScoreBoard", IE_Pressed, this, &ATaskPlayerControllerGameplay::CreateDestroyScoreWidget);
	InputComponent->BindAction("ScoreBoard", IE_Released, this, &ATaskPlayerControllerGameplay::CreateDestroyScoreWidget);

	InputComponent->BindAction("InGamePause", IE_Pressed, this, &ATaskPlayerControllerGameplay::CreateDestroyInGamePauseWidget);

		
}


void ATaskPlayerControllerGameplay::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ATaskPlayerControllerGameplay,PlayerTeam);
}
