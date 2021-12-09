#include "UI/SScoreBoardWidget.h"


#include "GameFramework/Player/TaskPlayerControllerGameplay.h"
#include "GameFramework/Player/TaskPlayerStateGameplay.h"
#include "GameFramework/Core/TaskGameStateGameplay.h"
#include "UI/SCScoreBoardWidget.h"
#include "Components/VerticalBox.h"
#include "Components/TextBlock.h"


void USScoreBoardWidget::OnScoresReceiveUpdate(int32 IBlueTeam, int32 IRedTeam)
{
	if (GSGameplay)
	{
		T_BlueTeamTotalScore->SetText(FText::FromString(FString::FromInt(GSGameplay->IBlueTeamScore)));

		T_BlueTeamTotalScore->SetText(FText::FromString(FString::FromInt(GSGameplay->IRedTeamScore)));
	}

}

void USScoreBoardWidget::OnKillScoreReceiveUpdate(ATaskPlayerStateGameplay* KillerState,ATaskPlayerStateGameplay* VictimState)
{
	FillScoreBoardChilds();
}


void USScoreBoardWidget::FillScoreBoardChilds()
{
	VB_BlueTeam->ClearChildren();
	VB_RedTeam->ClearChildren();

	const TArray<APlayerState*> PlayerStates= GetWorld()->GetGameState()->PlayerArray;
	
	for (APlayerState* PSPlayer : PlayerStates)
	{
		if (PSPlayer)
		{
			ATaskPlayerStateGameplay* TaskPSPlayer = Cast<ATaskPlayerStateGameplay>(PSPlayer);
			
			if (TaskPSPlayer)
			{
				USCScoreBoardWidget* SBChild=CreateWidget<USCScoreBoardWidget>(this,SBChildClass);
				
				switch (TaskPSPlayer->PlayerTeam)
				{
					case BlueTeam:
						VB_BlueTeam->AddChildToVerticalBox(SBChild);
						SBChild->SetupScoreBoardChildWidget(TaskPSPlayer);
						break;
					case RedTeam:
						VB_RedTeam->AddChildToVerticalBox(SBChild);
						SBChild->SetupScoreBoardChildWidget(TaskPSPlayer);
						break;
					
					default:	break;
				}
			}
		}
	}
}



void USScoreBoardWidget::NativeConstruct()
{
	GSGameplay=Cast<ATaskGameStateGameplay>(GetWorld()->GetGameState());

	if (GSGameplay)
	{
		FillScoreBoardChilds();

		T_BlueTeamTotalScore->SetText(FText::FromString(FString::FromInt(GSGameplay->IBlueTeamScore)));

		T_BlueTeamTotalScore->SetText(FText::FromString(FString::FromInt(GSGameplay->IRedTeamScore)));

	}
}


bool USScoreBoardWidget::Initialize()
{
	const bool Success=Super::Initialize();
	
	if(Success)
	{
		GSGameplay=Cast<ATaskGameStateGameplay>(GetWorld()->GetGameState());
		if (GSGameplay)
		{
			GSGameplay->OnTeamScoreChanged.AddDynamic(this,&USScoreBoardWidget::OnScoresReceiveUpdate);
		
			//GSGameplay->OnPlayerKill.AddDynamic(this,&USScoreBoardWidget::OnKillScoreReceiveUpdate);
		}
	}
	return Success;

}
