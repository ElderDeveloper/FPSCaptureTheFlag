#pragma once
#include "GameFramework/PlayerController.h"
#include "SGameplayData.h"
#include "TaskCharacter.h"
#include "TaskPlayerControllerGameplay.generated.h"


class ATaskPlayerStateGameplay;
class ATaskPlayerStateGameplay;
class USGameplayWidget;
class USScoreBoardWidget;
class USInGamePauseMenuWidget;
class USCKillNotifyWidget;
class USEndGameWidget;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAmmoChanged,int32,IAmmo);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHealthChanged,float,FHealthPercentage);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCaptureFlagStateChanged,bool,bHasFlag);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnPlayerDeath,FName,KillerName,TEnumAsByte<EPlayerTeam>,KillerTeam);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerRespawnCountdown,float,FTimeLeft);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPlayerRespawn);

UCLASS()
class ATaskPlayerControllerGameplay : public APlayerController
{
	GENERATED_BODY()
public:

	ATaskPlayerControllerGameplay(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(BlueprintReadWrite,Replicated)
	TEnumAsByte<EPlayerTeam> PlayerTeam;


	UPROPERTY(BlueprintAssignable)
	FOnAmmoChanged OnAmmoChanged;
	void SendOnAmmoChanged(int32 IAmmo);

	
	UPROPERTY(BlueprintAssignable)
	FOnHealthChanged OnHealthChanged;
	void SendOnHealthChanged(float IHealth , float IMaxHealth);

	
	UPROPERTY(BlueprintAssignable)
	FOnCaptureFlagStateChanged OnCaptureFlagStateChanged;
	void SendOnCaptureFlagStateChanged(bool bHasFlag);


	UPROPERTY(BlueprintAssignable)
	FOnPlayerDeath OnPlayerDeath;
	
	UPROPERTY(BlueprintAssignable)
	FOnPlayerRespawn OnPlayerRespawn;
	
	UPROPERTY(BlueprintAssignable)
	FOnPlayerRespawnCountdown OnPlayerRespawnCountdown;

	float FCurrentRespawnCountdown=0;

	UPROPERTY(EditDefaultsOnly)
	float FRespawnCountdown=5;

	FTimerHandle RespawnTimeHandle;



protected:
	// <<<<<<<<<<<<< Widget Variables >>>>>>>>>>>>>>>>
	// 
	//Gameplay Main Widget
	UPROPERTY()
	USGameplayWidget*WGameplayWidget;
	TSubclassOf<USGameplayWidget> GameplayWidgetClass;

	UFUNCTION(BlueprintCallable)
	void CreateGameplayWidget();

	
	//Score Board Setup
	UPROPERTY()
	USScoreBoardWidget*WScoreBoardWidget;
	TSubclassOf<USScoreBoardWidget> ScoreBoardClass;

	UPROPERTY()
	USEndGameWidget*WEndGameWidget;
	TSubclassOf<USEndGameWidget>EndGameWidgetClass;

	UFUNCTION(BlueprintCallable)
	void CreateDestroyScoreWidget();

	
	//In Game Pause Menu Setup
	UPROPERTY()
	USInGamePauseMenuWidget*WInGamePauseWidget;
	TSubclassOf<USInGamePauseMenuWidget> InGamePauseClass;
	UFUNCTION()
	void InGamePauseMenuDeconstruct();
	UFUNCTION(BlueprintCallable)
    void CreateDestroyInGamePauseWidget();

	
	//Kill Notify Class
	UPROPERTY(EditDefaultsOnly , Category="Widget")
	TSubclassOf<USCKillNotifyWidget> KillNotifyClass;

public:
	
	// <<<<<<<<<<<<<< Team Select And Respawn >>>>>>>>>>>>>>>>>>>>>>>>>
	UFUNCTION(BlueprintCallable)
	void RequestTeamSelect();

	UFUNCTION(Server,Reliable)
	void Server_RequestTeamSelect();

	void TeamSelectReceived(TEnumAsByte<EPlayerTeam> ReceivedTeam);

	UFUNCTION()
	void EndGameWidgetTrigger(TEnumAsByte<EPlayerTeam> WinnerTeam);
	
	UFUNCTION(Client,Reliable)
	void Client_ShowEndGameWidget(uint8 WinnerTeam);
	
	UFUNCTION(BlueprintCallable)
	void RequestRespawnPlayer();

	void RespawnCountdown();

	UFUNCTION(BlueprintImplementableEvent)
	void ReceivedSpawnedCharacter(ACharacter* SpawnedCharacter);
	
	UFUNCTION(Server,Reliable)
	void Server_RequestRespawnPlayer();

	ATaskPlayerStateGameplay* ReturnPlayerStateAsGameplay();

protected:

	
	virtual void BeginPlay() override;

	virtual void SetupInputComponent() override;
	
	virtual void GetLifetimeReplicatedProps(TArray < FLifetimeProperty > & OutLifetimeProps) const override;
};
