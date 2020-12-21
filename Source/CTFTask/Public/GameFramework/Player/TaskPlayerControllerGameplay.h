#pragma once
#include "GameFramework/PlayerController.h"
#include "SGameplayData.h"
#include "TaskCharacter.h"



#include "TaskPlayerControllerGameplay.generated.h"


class ATaskPlayerStateGameplay;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAmmoChanged,int32,IAmmo);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHealthChanged,float,FHealthPercentage);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCaptureFlagStateChanged,bool,bHasFlag);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerDeath,ATaskPlayerStateGameplay*,KillerPState);

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
	void SendOnPlayerDeath(ATaskPlayerStateGameplay*KillerPState);

	UPROPERTY(BlueprintAssignable)
	FOnPlayerRespawn OnPlayerRespawn;
	
	UPROPERTY(BlueprintAssignable)
	FOnPlayerRespawnCountdown OnPlayerRespawnCountdown;

	float FCurrentRespawnCountdown=0;

	UPROPERTY(EditDefaultsOnly)
	float FRespawnCountdown=10;

	FTimerHandle RespawnTimeHandle;



	UPROPERTY()
	class USGameplayWidget*WGameplayWidget;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class USGameplayWidget> WidgetClass;

	UFUNCTION(BlueprintCallable)
	void CreateGameplayWidget();
	
	// <<<<<<<<<<<<<< Team Select And Respawn >>>>>>>>>>>>>>>>>>>>>>>>>
	UFUNCTION(BlueprintCallable)
	void RequestTeamSelect();

	UFUNCTION(Server,Reliable)
	void Server_RequestTeamSelect();

	void TeamSelectReceived(TEnumAsByte<EPlayerTeam> ReceivedTeam);

	UFUNCTION(BlueprintCallable)
	void RequestRespawnPlayer();

	UFUNCTION(BlueprintImplementableEvent)
	void ReceivedSpawnedCharacter(ACharacter* SpawnedCharacter);
	
	UFUNCTION(Server,Reliable)
	void Server_RequestRespawnPlayer();

protected:



	UPROPERTY(EditDefaultsOnly)
	float RespawnTime = 3;
	
	virtual void BeginPlay() override;

	virtual void GetLifetimeReplicatedProps(TArray < FLifetimeProperty > & OutLifetimeProps) const override;
};
