#pragma once

#include "Blueprint/UserWidget.h"
#include "SInGamePauseMenuWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDeconstruct);


class UTextBlock;
class UButton;
UCLASS(BlueprintType,Blueprintable)
class USInGamePauseMenuWidget : public UUserWidget
{

	GENERATED_BODY()
public:
	
	FOnDeconstruct OnDeconstruct;

protected:

	UPROPERTY(BlueprintReadWrite,meta = (BindWidget))
	UTextBlock*T_ResumeGame;

	UPROPERTY(BlueprintReadWrite,meta = (BindWidget))
	UTextBlock*T_MainMenu;

	UPROPERTY(BlueprintReadWrite,meta = (BindWidget))
	UTextBlock*T_ExitGame;

	UPROPERTY(BlueprintReadWrite,meta = (BindWidget))
	UButton*BTN_Resume;

	UPROPERTY(BlueprintReadWrite,meta = (BindWidget))
	UButton*BTN_MainMenu;

	UPROPERTY(BlueprintReadWrite,meta = (BindWidget))
	UButton*BTN_ExitGame;


	UFUNCTION()
	void Resume_OnHovered();
	UFUNCTION()
	void Resume_OnUnHovered();
	UFUNCTION()
	void Resume_OnClicked();

	UFUNCTION()
	void MainMenu_OnHovered();
	UFUNCTION()
	void MainMenu_OnUnHovered();
	UFUNCTION()
	void MainMenu_OnClicked();

	UFUNCTION()
	void ExitGame_OnHovered();
	UFUNCTION()
	void ExitGame_OnUnHovered();
	UFUNCTION()
	void ExitGame_OnClicked();
	
	virtual void NativeConstruct() override;

	virtual bool Initialize() override;

	virtual void NativeDestruct() override;
	
};
