#include "UI/SCScoreBoardWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "GameFramework/Player/TaskPlayerStateGameplay.h"



void USCScoreBoardWidget::SetupScoreBoardChildWidget(ATaskPlayerStateGameplay* PSOwner)
{
	if (PSOwner)
	{

	T_PlayerName->SetText(FText::FromString(PSOwner->GetPlayerName()));

	T_PlayerName->SetColorAndOpacity(PSOwner->PlayerTeam==BlueTeam ? BlueTeamColor : RedTeamColor);

	T_PlayerKillCount->SetText(FText::FromString(FString::FromInt(PSOwner->IPlayerKillCount)));

	Img_DeathState->SetVisibility(PSOwner->bIsAlive ? ESlateVisibility::Hidden : ESlateVisibility::Visible);

	//T_Ping->SetText(FText::FromString(BytesToStringFixed(&PSOwner->Ping,100)));
		
	}
	
}
