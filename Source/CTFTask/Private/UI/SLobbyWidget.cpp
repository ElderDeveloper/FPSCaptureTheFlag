#include "UI/SLobbyWidget.h"
#include "Components/VerticalBox.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "GameFramework/Core/Lobby/TaskGameModeLobby.h"
#include "Library/FPSGameLibrary.h"
#include "UMG/Public/Blueprint/UserWidget.h"

/*
USLobbyWidget::USLobbyWidget()
{
	
	const ConstructorHelpers::FClassFinder<UUserWidget> LobbyChildConstruct (TEXT("/Game/UI/W_InGamePauseMenu"));
	if(!ensure(LobbyChildConstruct.Class!= nullptr)) return;
	PlayerWidgetClass=LobbyChildConstruct.Class;
	
}
*/
void USLobbyWidget::ClearPlayerWindow()
{
	VB_PlayersWindow->ClearChildren();
}

void USLobbyWidget::CountdownReceived(int32 Countdown)
{
	switch (Countdown)
	{
		case 0 :
			T_Countdown->SetVisibility(ESlateVisibility::Hidden);
			break;
		default:
			T_Countdown->SetVisibility(ESlateVisibility::Visible);
			T_Countdown->SetText(FText::FromString(FString::FromInt(Countdown)));
			break;
	}
}

void USLobbyWidget::UpdatePlayersWindow_Implementation(FName PlayerName, bool PlayerReady, bool CanChangeState)
{
	USCLobbyPlayerWidget* LobbyPlayerWidget = CreateWidget<USCLobbyPlayerWidget>(GetOwningPlayer(),PlayerWidgetClass);
	if (LobbyPlayerWidget)
	{
		VB_PlayersWindow->AddChildToVerticalBox(LobbyPlayerWidget);
		LobbyPlayerWidget->SetupWidget(PlayerName,PlayerReady,CanChangeState);
	}
}



void USLobbyWidget::BTN_ExitLobby_OnHovered()
{	T_ExitLobby->SetColorAndOpacity(UFPSGameLibrary::GetUIButtonTextColor(GetWorld(),"Hovered"));	}

void USLobbyWidget::BTN_ExitLobby_OnUnHovered()
{	T_ExitLobby->SetColorAndOpacity(UFPSGameLibrary::GetUIButtonTextColor(GetWorld(),"UnHovered"));	}

void USLobbyWidget::BTN_ExitLobby_OnClicked()
{
}



void USLobbyWidget::NativeConstruct()
{
	Super::NativeConstruct();
}


bool USLobbyWidget::Initialize()
{
	const bool Success=Super::Initialize();
	
	if(Success)
	{

		if(!ensure(BTN_ExitLobby!=nullptr))	return false;
		BTN_ExitLobby->OnHovered.AddDynamic(this,&USLobbyWidget::BTN_ExitLobby_OnHovered);
		BTN_ExitLobby->OnUnhovered.AddDynamic(this,&USLobbyWidget::BTN_ExitLobby_OnUnHovered);
		BTN_ExitLobby->OnClicked.AddDynamic(this,&USLobbyWidget::BTN_ExitLobby_OnClicked);
		
	}

	return Success;
}


void USLobbyWidget::NativeDestruct()
{
	Super::NativeDestruct();

	
}
