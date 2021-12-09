#include "UI/SCKillNotifyWidget.h"
#include "Components/TextBlock.h"
#include "GameFramework/Player/TaskPlayerStateGameplay.h"

void USCKillNotifyWidget::SetupWidget(ATaskPlayerStateGameplay* PSKiller, ATaskPlayerStateGameplay* PSVictim)
{
	if (PSKiller && PSVictim)
	{
		if (PSKiller->PlayerTeam!=None && PSVictim->PlayerTeam!=None)
		{
			T_Killer->SetText(FText::FromString(PSKiller->GetPlayerName()));
			
			T_Killer->SetColorAndOpacity(PSKiller->PlayerTeam==BlueTeam ? BlueTeamColor : RedTeamColor);

			T_Victim->SetText(FText::FromString(PSVictim->GetPlayerName()));
			
			T_Victim->SetColorAndOpacity(PSVictim->PlayerTeam==BlueTeam ? BlueTeamColor : RedTeamColor);

			GetWorld()->GetTimerManager().SetTimer(DestroyTimeHandle,this,&USCKillNotifyWidget::RemoveFromParent,FLifeTime);
		}

		else
		{
			RemoveFromParent();
		}
	}
	else
	{
		RemoveFromParent();
	}
	
}
