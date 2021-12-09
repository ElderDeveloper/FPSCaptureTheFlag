#pragma once

#include "Blueprint/UserWidget.h"
#include "SHomeScreenWidget.generated.h"


class UTextBlock;
class UButton;
UCLASS(BlueprintType,Blueprintable)
class USHomeScreenWidget : public UUserWidget
{

	GENERATED_BODY()
public:

protected:
	UPROPERTY(BlueprintReadWrite,meta = (BindWidget))
	UButton*BTN_HostGame;
	
	UPROPERTY(BlueprintReadWrite,meta = (BindWidget))
	UButton*BTN_JoinGame;

	UPROPERTY(BlueprintReadWrite,meta = (BindWidget))
	UButton*BTN_ExitGame;

	UPROPERTY(BlueprintReadWrite,meta = (BindWidget))
	UTextBlock*T_HostGame;
	
	UPROPERTY(BlueprintReadWrite,meta = (BindWidget))
	UTextBlock*T_JoinGame;

	UPROPERTY(BlueprintReadWrite,meta = (BindWidget))
	UTextBlock*T_Exit;


	UFUNCTION()
	void BTN_HostGame_OnHovered();
	UFUNCTION()
    void Btn_HostGame_OnUnHovered();
	UFUNCTION()
    void BBTN_HostGame_OnClicked();


	UFUNCTION()
	void BTN_JoinGame_OnHovered();
	UFUNCTION()
    void BTN_JoinGame_OnUnHovered();
	UFUNCTION()
    void BTN_JoinGame_OnClicked();

	void OnFindSessionsComplete(const bool Success);
	
	UFUNCTION()
    void BTN_ExitGame_OnHovered();
	UFUNCTION()
    void BTN_ExitGame_OnUnHovered();
	UFUNCTION()
    void BTN_ExitGame_OnClicked();
	

	virtual bool Initialize() override;


	
};
