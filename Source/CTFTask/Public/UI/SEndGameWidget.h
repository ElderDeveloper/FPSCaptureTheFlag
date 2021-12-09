#pragma once

#include "Blueprint/UserWidget.h"
#include "SGameplayData.h"
#include "SEndGameWidget.generated.h"

class UTextBlock;
class UVerticalBox;
class USCScoreBoardWidget;
class ATaskPlayerStateGameplay;
class UButton;
UCLASS(BlueprintType,Blueprintable)
class USEndGameWidget : public UUserWidget
{

	GENERATED_BODY()


public:

	void SetupWidget(TEnumAsByte<EPlayerTeam> WinnerTeam);

protected:
	UPROPERTY(BlueprintReadWrite,meta = (BindWidget))
	UButton*BTN_MainMenu;

	UPROPERTY(BlueprintReadWrite,meta = (BindWidget))
	UTextBlock*T_MainMenu;

	UPROPERTY(BlueprintReadWrite,meta = (BindWidget))
	UTextBlock*T_BlueTeamTotalScore;
		
	UPROPERTY(BlueprintReadWrite,meta = (BindWidget))
	UTextBlock*T_RedTeamTotalScore;

	UPROPERTY(BlueprintReadWrite,meta = (BindWidget))
	UTextBlock*T_WinnerTeam;

	UFUNCTION()
	void BTN_MainMenu_OnHovered();
	UFUNCTION()
    void BTN_MainMenu_OnUnHovered();
	UFUNCTION()
    void BTN_MainMenu_OnClicked();
	
    UPROPERTY()
	class ATaskGameStateGameplay*GSGameplay;
	
	virtual void NativeConstruct() override;

	virtual bool Initialize() override;
	
};
