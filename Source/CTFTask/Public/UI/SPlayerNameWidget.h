#pragma once
#include "Blueprint/UserWidget.h"
#include "SPlayerNameWidget.generated.h"

UCLASS(BlueprintType,Blueprintable)
class USPlayerNameWidget : public UUserWidget
{
    GENERATED_BODY()
public:
    UPROPERTY(BlueprintReadWrite,meta = (BindWidget))
    class UTextBlock*T_PlayerName;
    virtual void NativeConstruct() override;
};
