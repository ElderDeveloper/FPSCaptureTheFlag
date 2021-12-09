#pragma once

#include "SCLobbyPlayerWidget.h"
#include "Blueprint/UserWidget.h"


#include "SLobbyWidget.generated.h"


class UTextBlock;
class UButton;
class UVerticalBox;

UCLASS(BlueprintType,Blueprintable)
class USLobbyWidget : public UUserWidget
{

	GENERATED_BODY()
public:

	UPROPERTY()
	USCLobbyPlayerWidget*PlayerWidget;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<USCLobbyPlayerWidget> PlayerWidgetClass;

	
	void ClearPlayerWindow();
	
	UFUNCTION(Client,Reliable)
	void UpdatePlayersWindow(FName PlayerName , bool PlayerReady , bool CanChangeState);

	UFUNCTION()
	void CountdownReceived(int32 Countdown);

protected:
	

	UPROPERTY(BlueprintReadWrite,meta = (BindWidget))
	UButton*BTN_ExitLobby;

	UPROPERTY(BlueprintReadWrite,meta = (BindWidget))
	UTextBlock*T_ExitLobby;

	UPROPERTY(BlueprintReadWrite,meta = (BindWidget))
	UVerticalBox*VB_PlayersWindow;

	UPROPERTY(BlueprintReadWrite,meta = (BindWidget))
	UTextBlock*T_Countdown;



	UFUNCTION()
    void BTN_ExitLobby_OnHovered();
	UFUNCTION()
    void BTN_ExitLobby_OnUnHovered();
	UFUNCTION()
    void BTN_ExitLobby_OnClicked();

	
	virtual void NativeConstruct() override;

	virtual bool Initialize() override;

	virtual void NativeDestruct() override;
	
};
