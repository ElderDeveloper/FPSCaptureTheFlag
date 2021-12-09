#include "UI/SPlayerNameWidget.h"
#include "Components/TextBlock.h"
#include "GameFramework/PlayerState.h"

void USPlayerNameWidget::NativeConstruct()
{
    if (GetOwningPlayer())
    {
        T_PlayerName->SetText(FText::FromString(GetOwningPlayer()->PlayerState->GetPlayerName()));
    }

}
