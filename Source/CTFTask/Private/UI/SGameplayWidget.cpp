#include "UI/SGameplayWidget.h"
#include "Components/TextBlock.h"
#include "Components/CanvasPanel.h"
#include "Components/ProgressBar.h"
#include "Components/Image.h"
#include "Components/VerticalBox.h"
#include "GameFramework/Core/TaskGameStateGameplay.h"
#include "GameFramework/Player/TaskPlayerStateGameplay.h"
#include "GameFramework/Player/TaskPlayerControllerGameplay.h"
#include "UI/SCKillNotifyWidget.h"


void USGameplayWidget::OnUpdateGameScores(int32 IBlueTeam, int32 IRedTeam)
{
	T_BlueTeamScore->SetText(FText::FromString(FString::FromInt(IBlueTeam)));
	T_RedTeamScore->SetText(FText::FromString(FString::FromInt(IRedTeam)));
}


void USGameplayWidget::OnUpdateGameTimeLeft(float FTimeLeft)
{
	FString Time=FString::SanitizeFloat(FTimeLeft,0);
	T_GameTimeLeft->SetText(FText::FromString(Time));
}


void USGameplayWidget::OnUpdateWeaponAmmo(int32 IAmmo)
{
	T_WeaponAmmo->SetText(FText::FromString(FString::FromInt(IAmmo)));
}


void USGameplayWidget::OnCaptureFlagStateChanged(bool bIsCarrying)
{
	Img_CarryingFlag->SetVisibility(bIsCarrying ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
}


void USGameplayWidget::OnUpdatePlayerHealth(float FHealthPercent)
{
	PB_PlayerHeath->SetPercent(FHealthPercent);
}


void USGameplayWidget::OnReceiveKillNotify(ATaskPlayerStateGameplay* KillerState, ATaskPlayerStateGameplay* VictimState)
{
	USCKillNotifyWidget*Kill=CreateWidget<USCKillNotifyWidget>(GetOwningPlayer(),KillClass);
	if (Kill)
	{
		VB_KillNotify->AddChildToVerticalBox(Kill);
		Kill->SetupWidget(KillerState,VictimState);
	}
}


void USGameplayWidget::OnPlayerDeath(FName KillerName,TEnumAsByte<EPlayerTeam> KillerTeam)
{
	
	if (PlayerControllerGameplay)
	{
		FString Time=FString::SanitizeFloat(PlayerControllerGameplay->FRespawnCountdown,0);
		T_RespawnCountdown->SetText(FText::FromString(Time));
	}

	Img_CarryingFlag->SetVisibility(ESlateVisibility::Hidden);

	T_KillerName->SetColorAndOpacity(KillerTeam == BlueTeam ? SCBlueTeamColor : SCRedTeamColor);

	T_KillerName->SetText(FText::FromString(KillerName.ToString()));

	
	DeathScreenCanvas->SetVisibility(ESlateVisibility::Visible);

	PlayerCanvas->SetVisibility(ESlateVisibility::Collapsed);

}


void USGameplayWidget::OnPlayerRespawnCountdown(float FTimeLeft)
{
	FString Time=FString::SanitizeFloat(FTimeLeft,0);
	T_RespawnCountdown->SetText(FText::FromString(Time));
}


void USGameplayWidget::OnPlayerRespawn()
{
	DeathScreenCanvas->SetVisibility(ESlateVisibility::Collapsed);

	PlayerCanvas->SetVisibility(ESlateVisibility::Visible);

	PB_PlayerHeath->SetPercent(1);

	T_WeaponAmmo->SetText(FText::FromString("48"));
}


void USGameplayWidget::NativeConstruct()
{
	Super::NativeConstruct();
}


bool USGameplayWidget::Initialize()
{
	const bool Success=Super::Initialize();
	if(Success)
	{
		ATaskGameStateGameplay* GameStateGameplay = Cast<ATaskGameStateGameplay>(GetWorld()->GetGameState());

		if (GameStateGameplay)
		{
			GameStateGameplay->OnGameTimeChanged.AddDynamic(this,&USGameplayWidget::OnUpdateGameTimeLeft);
			GameStateGameplay->OnTeamScoreChanged.AddDynamic(this,&USGameplayWidget::OnUpdateGameScores);
			GameStateGameplay->OnPlayerKill.AddDynamic(this,&USGameplayWidget::OnReceiveKillNotify);
		}

		PlayerControllerGameplay = Cast<ATaskPlayerControllerGameplay>(GetOwningPlayer());

		if (PlayerControllerGameplay)
		{
			PlayerControllerGameplay->OnAmmoChanged.AddDynamic(this,&USGameplayWidget::OnUpdateWeaponAmmo);
			PlayerControllerGameplay->OnHealthChanged.AddDynamic(this,&USGameplayWidget::OnUpdatePlayerHealth);
			PlayerControllerGameplay->OnCaptureFlagStateChanged.AddDynamic(this,&USGameplayWidget::OnCaptureFlagStateChanged);
			PlayerControllerGameplay->OnPlayerDeath.AddDynamic(this,&USGameplayWidget::OnPlayerDeath);
			PlayerControllerGameplay->OnPlayerRespawn.AddDynamic(this,&USGameplayWidget::OnPlayerRespawn);
			PlayerControllerGameplay->OnPlayerRespawnCountdown.AddDynamic(this,&USGameplayWidget::OnPlayerRespawnCountdown);
		}
	}

	return Success;
}
