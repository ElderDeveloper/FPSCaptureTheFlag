#include "UI/SCLobbyPlayerWidget.h"
#include "Components/CheckBox.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "GameFramework/Core/Lobby/TaskPlayerControllerLobby.h"



void USCLobbyPlayerWidget::SetupWidget(FName PlayerName, bool PlayerReady, bool CanChangeState)
{
    T_PlayerName->SetText(FText::FromString(PlayerName.ToString()));
    
    NPlayerName=PlayerName;
    
    Checkbox_Ready->SetIsChecked(PlayerReady);

    Checkbox_Ready->SetIsEnabled(CanChangeState);
}

void USCLobbyPlayerWidget::CheckboxStateChanged(bool bIsChecked )
{
    ATaskPlayerControllerLobby*PC = Cast<ATaskPlayerControllerLobby>(GetOwningPlayer());
    
    if(PC)
    {
        PC->Server_CallUpdateAllPlayers(NPlayerName,bIsChecked);
    }
}

void USCLobbyPlayerWidget::NativeConstruct()
{
    Super::NativeConstruct();
    
}

bool USCLobbyPlayerWidget::Initialize()
{
     const bool Success=Super::Initialize();
     if (!Success)    return Success;

    Checkbox_Ready->OnCheckStateChanged.AddDynamic(this,&USCLobbyPlayerWidget::CheckboxStateChanged);

    return Success;
 
}
