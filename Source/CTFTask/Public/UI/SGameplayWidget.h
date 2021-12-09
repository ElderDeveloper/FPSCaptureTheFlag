#pragma once


#include "Blueprint/UserWidget.h"
#include "SGameplayData.h"
#include "SGameplayWidget.generated.h"

class UCanvasPanel;
class UTextBlock;
class UProgressBar;
class UImage;
class UVerticalBox;
class ATaskPlayerStateGameplay;
UCLASS(BlueprintType,Blueprintable)
class USGameplayWidget : public UUserWidget
{

	GENERATED_BODY()

protected:

	
	//<<<<<<<<<<<<<<<<< Canvas >>>>>>>>>>>>>>>>>>>>>>>
	UPROPERTY(BlueprintReadWrite,meta = (BindWidget))
	UCanvasPanel*GameStateCanvas;

	UPROPERTY(BlueprintReadWrite,meta = (BindWidget))
	UCanvasPanel*PlayerCanvas;
	
	UPROPERTY(BlueprintReadWrite,meta = (BindWidget))
	UCanvasPanel*DeathScreenCanvas;

	//<<<<<<<<<<<<<<<<<<<<< Text Blocks >>>>>>>>>>>>>>>>>>>>>>>
	UPROPERTY(BlueprintReadWrite,meta = (BindWidget))
	UTextBlock*T_BlueTeamScore;

	UPROPERTY(BlueprintReadWrite,meta = (BindWidget))
	UTextBlock*T_RedTeamScore;

	UPROPERTY(BlueprintReadWrite,meta = (BindWidget))
	UTextBlock*T_GameTimeLeft;

	UPROPERTY(BlueprintReadWrite,meta = (BindWidget))
	UTextBlock*T_WeaponAmmo;

	UPROPERTY(BlueprintReadWrite,meta = (BindWidget))
	UTextBlock*T_RespawnCountdown;

	UPROPERTY(BlueprintReadWrite,meta = (BindWidget))
	UTextBlock*T_KillerName;

	//<<<<<<<<<<<<<<<<<<< Images >>>>>>>>>>>>>>>>>>>>
	UPROPERTY(BlueprintReadWrite,meta = (BindWidget))
	UImage*Img_CarryingFlag;

	UPROPERTY(BlueprintReadWrite,meta = (BindWidget))
	UImage*Img_CrosshairUp;
	
	UPROPERTY(BlueprintReadWrite,meta = (BindWidget))
	UImage*Img_CrosshairDown;
	
	UPROPERTY(BlueprintReadWrite,meta = (BindWidget))
	UImage*Img_CrosshairLeft;
	
	UPROPERTY(BlueprintReadWrite,meta = (BindWidget))
	UImage*Img_CrosshairRight;


	//<<<<<<<<<<<<<<<<<<< Others >>>>>>>>>>>>>>>>>>>>>>

	UPROPERTY(BlueprintReadWrite,meta = (BindWidget))
	UProgressBar*PB_PlayerHeath;

	UPROPERTY(BlueprintReadWrite,meta = (BindWidget))
	UVerticalBox*VB_KillNotify;


	// <<<<<<<<<<<<<<<<<<<<<< Variables >>>>>>>>>>>>>>>>>>>>>>
	UPROPERTY()
	class ATaskPlayerControllerGameplay* PlayerControllerGameplay;

	UPROPERTY(EditDefaultsOnly)
	FSlateColor SCRedTeamColor;

	UPROPERTY(EditDefaultsOnly)
	FSlateColor SCBlueTeamColor;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class USCKillNotifyWidget> KillClass;

public:


	UFUNCTION()
	void OnUpdateGameScores(int32 IBlueTeam,int32 IRedTeam);

	UFUNCTION()
	void OnUpdateGameTimeLeft(float FTimeLeft);
	
	UFUNCTION()
	void OnUpdateWeaponAmmo(int32 IAmmo);

	UFUNCTION()
	void OnCaptureFlagStateChanged(bool bIsCarrying);

	UFUNCTION()
	void OnUpdatePlayerHealth(float FHealthPercent);

	UFUNCTION()
	void OnReceiveKillNotify(ATaskPlayerStateGameplay* KillerState , ATaskPlayerStateGameplay* VictimState);

	UFUNCTION()
	void OnPlayerDeath(FName KillerName,TEnumAsByte<EPlayerTeam> KillerTeam);

	UFUNCTION()
	void OnPlayerRespawnCountdown(float FTimeLeft);

	UFUNCTION()
	void OnPlayerRespawn();

protected:

	virtual void NativeConstruct() override;

	virtual bool Initialize() override;
	
};
