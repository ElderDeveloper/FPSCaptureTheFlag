#include "UI/SInGamePauseMenuWidget.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Library/FPSGameLibrary.h"
#include "UMG/Public/Blueprint/WidgetBlueprintLibrary.h"


void USInGamePauseMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();
	APlayerController* Own=GetOwningPlayer();
	if (Own)
	{
		UWidgetBlueprintLibrary::SetInputMode_UIOnlyEx(Own,this);
		Own->bShowMouseCursor=true;
	}
}

bool USInGamePauseMenuWidget::Initialize()
{
	const bool Success = Super::Initialize();
	if(Success)
	{
		if(!ensure(BTN_Resume!=nullptr))	return false;
		BTN_Resume->OnHovered.AddDynamic(this,&USInGamePauseMenuWidget::Resume_OnHovered);
		BTN_Resume->OnUnhovered.AddDynamic(this,&USInGamePauseMenuWidget::Resume_OnUnHovered);
		BTN_Resume->OnClicked.AddDynamic(this,&USInGamePauseMenuWidget::Resume_OnClicked);

		if(!ensure(BTN_MainMenu!=nullptr))	return false;
		BTN_MainMenu->OnHovered.AddDynamic(this,&USInGamePauseMenuWidget::MainMenu_OnHovered);
		BTN_MainMenu->OnUnhovered.AddDynamic(this,&USInGamePauseMenuWidget::MainMenu_OnUnHovered);
		BTN_MainMenu->OnClicked.AddDynamic(this,&USInGamePauseMenuWidget::MainMenu_OnClicked);

		if(!ensure(BTN_ExitGame!=nullptr))	return false;
		BTN_ExitGame->OnHovered.AddDynamic(this,&USInGamePauseMenuWidget::ExitGame_OnHovered);
		BTN_ExitGame->OnUnhovered.AddDynamic(this,&USInGamePauseMenuWidget::ExitGame_OnUnHovered);
		BTN_ExitGame->OnClicked.AddDynamic(this,&USInGamePauseMenuWidget::ExitGame_OnClicked);
	}

	
	return Success;
}

void USInGamePauseMenuWidget::NativeDestruct()
{
	Super::NativeDestruct();
	
	APlayerController* Own=GetOwningPlayer();
	if (Own)
	{
		//const FInputModeGameOnly InputMode;
		//Own->SetInputMode(InputMode);
		UWidgetBlueprintLibrary::SetInputMode_GameOnly(Own);
		Own->bShowMouseCursor=false;
		OnDeconstruct.Broadcast();
	}
}



void USInGamePauseMenuWidget::Resume_OnHovered()
{	T_ResumeGame->SetColorAndOpacity(UFPSGameLibrary::GetUIButtonTextColor(GetWorld(),"Hovered"));	}

void USInGamePauseMenuWidget::Resume_OnUnHovered()
{	T_ResumeGame->SetColorAndOpacity(UFPSGameLibrary::GetUIButtonTextColor(GetWorld(),"UnHovered"));	}

void USInGamePauseMenuWidget::Resume_OnClicked()	{	RemoveFromParent();	}



void USInGamePauseMenuWidget::MainMenu_OnHovered()
{	T_MainMenu->SetColorAndOpacity(UFPSGameLibrary::GetUIButtonTextColor(GetWorld(),"Hovered"));	}

void USInGamePauseMenuWidget::MainMenu_OnUnHovered()
{	T_MainMenu->SetColorAndOpacity(UFPSGameLibrary::GetUIButtonTextColor(GetWorld(),"UnHovered"));	}

void USInGamePauseMenuWidget::MainMenu_OnClicked()
{
	UGameplayStatics::OpenLevel(GetWorld(),FName("MainMenu"));
}



void USInGamePauseMenuWidget::ExitGame_OnHovered()
{	T_ExitGame->SetColorAndOpacity(UFPSGameLibrary::GetUIButtonTextColor(GetWorld(),"Hovered"));	}

void USInGamePauseMenuWidget::ExitGame_OnUnHovered()
{	T_ExitGame->SetColorAndOpacity(UFPSGameLibrary::GetUIButtonTextColor(GetWorld(),"UnHovered"));	}

void USInGamePauseMenuWidget::ExitGame_OnClicked()
{
	UKismetSystemLibrary::QuitGame(GetWorld(),GetOwningPlayer(),EQuitPreference::Quit,true);
}

