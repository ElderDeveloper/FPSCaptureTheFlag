#include "UI/SHomeScreenWidget.h"

#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Engine/TaskGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Library/FPSGameLibrary.h"


void USHomeScreenWidget::BTN_HostGame_OnHovered()
{	T_HostGame->SetColorAndOpacity(UFPSGameLibrary::GetUIButtonTextColor(GetWorld(),"Hovered"));	}

void USHomeScreenWidget::Btn_HostGame_OnUnHovered()
{	T_HostGame->SetColorAndOpacity(UFPSGameLibrary::GetUIButtonTextColor(GetWorld(),"UnHovered"));	}

void USHomeScreenWidget::BBTN_HostGame_OnClicked()
{
}

void USHomeScreenWidget::BTN_JoinGame_OnHovered()
{	T_JoinGame->SetColorAndOpacity(UFPSGameLibrary::GetUIButtonTextColor(GetWorld(),"Hovered"));	}

void USHomeScreenWidget::BTN_JoinGame_OnUnHovered()
{	T_JoinGame->SetColorAndOpacity(UFPSGameLibrary::GetUIButtonTextColor(GetWorld(),"UnHovered"));	}



void USHomeScreenWidget::BTN_JoinGame_OnClicked()
{
	/*
	UTaskGameInstance*GI= Cast<UTaskGameInstance>(GetWorld()->GetGameInstance());
	if(GI)
	{
		//GI->SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this,&USHomeScreenWidget::OnFindSessionsComplete);

		GI->FindSessions();

		BTN_Matchmaking->SetIsEnabled(false);

		T_Matchmaking->SetText(FText::FromString("Looking For Games"));
	}
	*/
}



void USHomeScreenWidget::OnFindSessionsComplete(const bool Success)
{
	BTN_JoinGame->SetIsEnabled(true);
	T_JoinGame->SetText(FText::FromString("Matchmaking"));

	
}






void USHomeScreenWidget::BTN_ExitGame_OnHovered()
{	T_Exit->SetColorAndOpacity(UFPSGameLibrary::GetUIButtonTextColor(GetWorld(),"Hovered"));	}



void USHomeScreenWidget::BTN_ExitGame_OnUnHovered()
{	T_Exit->SetColorAndOpacity(UFPSGameLibrary::GetUIButtonTextColor(GetWorld(),"UnHovered"));	}



void USHomeScreenWidget::BTN_ExitGame_OnClicked()
{
	UKismetSystemLibrary::QuitGame(GetWorld(),GetOwningPlayer(),EQuitPreference::Quit,true);
}



bool USHomeScreenWidget::Initialize()
{
	const bool Success=Super::Initialize();
	
	if(Success)
	{
		if(!ensure(BTN_HostGame!=nullptr))	return false;
		BTN_HostGame->OnHovered.AddDynamic(this,&USHomeScreenWidget::BTN_HostGame_OnHovered);
		BTN_HostGame->OnUnhovered.AddDynamic(this,&USHomeScreenWidget::Btn_HostGame_OnUnHovered);
		BTN_HostGame->OnClicked.AddDynamic(this,&USHomeScreenWidget::BBTN_HostGame_OnClicked);

		if(!ensure(BTN_JoinGame!=nullptr))	return false;
		BTN_JoinGame->OnHovered.AddDynamic(this,&USHomeScreenWidget::BTN_JoinGame_OnHovered);
		BTN_JoinGame->OnUnhovered.AddDynamic(this,&USHomeScreenWidget::BTN_JoinGame_OnUnHovered);
		BTN_JoinGame->OnClicked.AddDynamic(this,&USHomeScreenWidget::BTN_JoinGame_OnClicked);

		if(!ensure(BTN_ExitGame!=nullptr))	return false;
		BTN_ExitGame->OnHovered.AddDynamic(this,&USHomeScreenWidget::BTN_ExitGame_OnHovered);
		BTN_ExitGame->OnUnhovered.AddDynamic(this,&USHomeScreenWidget::BTN_ExitGame_OnUnHovered);
		BTN_ExitGame->OnClicked.AddDynamic(this,&USHomeScreenWidget::BTN_ExitGame_OnClicked);
		
	}

	return Success;
}

