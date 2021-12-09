#pragma once

#include "Blueprint/UserWidget.h"


#include "SCLobbyPlayerWidget.generated.h"

class UTextBlock;
class UImage;
class UCheckBox;

UCLASS(BlueprintType,Blueprintable)
class USCLobbyPlayerWidget : public UUserWidget
{
	GENERATED_BODY()
public:

	void SetupWidget(FName PlayerName, bool PlayerReady, bool CanChangeState);
	
	UPROPERTY(BlueprintReadWrite,meta = (BindWidget))
	UTextBlock*T_PlayerName;

	UPROPERTY(BlueprintReadWrite,meta = (BindWidget))
	UCheckBox*Checkbox_Ready;

	UPROPERTY()
	APlayerState*OwnerPlayerState;

	FName NPlayerName;

	UFUNCTION()
	void CheckboxStateChanged(bool bIsChecked);

	virtual void NativeConstruct() override;

	virtual bool Initialize() override;
};
