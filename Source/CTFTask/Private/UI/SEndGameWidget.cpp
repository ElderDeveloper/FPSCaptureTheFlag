#include "UI/SEndGameWidget.h"



#include "Components/Button.h"
#include "GameFramework/Core/TaskGameStateGameplay.h"
#include "UI/SCScoreBoardWidget.h"
#include "Components/TextBlock.h"
#include "Library/FPSGameLibrary.h"


void USEndGameWidget::SetupWidget(TEnumAsByte<EPlayerTeam> WinnerTeam)
{
	switch (WinnerTeam)
	{
		case BlueTeam:
			T_WinnerTeam->SetText(FText::FromString("BLUE TEAM"));
			break;
		case RedTeam:
			T_WinnerTeam->SetText(FText::FromString("RED TEAM"));
			break;
		case None:
			T_WinnerTeam->SetText(FText::FromString("DRAW"));
			break;
		default:break;
	}

	ATaskGameStateGameplay*GameStateGameplay = Cast<ATaskGameStateGameplay>(GetWorld()->GetGameState());
	if(GameStateGameplay)
	{
		T_BlueTeamTotalScore->SetText(FText::FromString(FString::FromInt(GameStateGameplay->IBlueTeamScore)));
		T_RedTeamTotalScore->SetText(FText::FromString(FString::FromInt(GameStateGameplay->IRedTeamScore)));
	}
}

void USEndGameWidget::BTN_MainMenu_OnHovered()
{
	T_MainMenu->SetColorAndOpacity(UFPSGameLibrary::GetUIButtonTextColor(GetWorld(),"Hovered"));
}

void USEndGameWidget::BTN_MainMenu_OnUnHovered()
{
	T_MainMenu->SetColorAndOpacity(UFPSGameLibrary::GetUIButtonTextColor(GetWorld(),"UnHovered"));
}

void USEndGameWidget::BTN_MainMenu_OnClicked()
{

}

void USEndGameWidget::NativeConstruct()
{
	GSGameplay=Cast<ATaskGameStateGameplay>(GetWorld()->GetGameState());

}


bool USEndGameWidget::Initialize()
{
	const bool Success=Super::Initialize();
	if(Success)
	{
		if(!ensure(BTN_MainMenu!=nullptr))	return false;
		BTN_MainMenu->OnHovered.AddDynamic(this,&USEndGameWidget::BTN_MainMenu_OnHovered);
		BTN_MainMenu->OnUnhovered.AddDynamic(this,&USEndGameWidget::BTN_MainMenu_OnUnHovered);
		BTN_MainMenu->OnClicked.AddDynamic(this,&USEndGameWidget::BTN_MainMenu_OnClicked);
	}

	return Success;

}
