#pragma once
#include "GameFramework/GameStateBase.h"
#include "SGameplayData.h"

#include "TaskGameStateGameplay.generated.h"

class ATaskPlayerControllerGameplay;
class ATaskPlayerStateGameplay;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnTeamScoreChanged,int32,IBlueTeam,int32,IRedTeam);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGameTimeChanged,float,FGameTime);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnPlayerKill,ATaskPlayerStateGameplay*,KillerState,ATaskPlayerStateGameplay*,VictimState);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEndGame,TEnumAsByte<EPlayerTeam>,WinnerTeam);

UCLASS()
class ATaskGameStateGameplay : public AGameStateBase
{
	GENERATED_BODY()
public:


	
	ATaskGameStateGameplay();
	

	void Server_AddNewPlayerToTheTeam(TEnumAsByte<EPlayerTeam> PlayerT , class ATaskPlayerControllerGameplay* PlayerC);
	
	
	UPROPERTY(Replicated,BlueprintReadOnly)
	int32 IBlueTeamPlayerCount=0;

	UPROPERTY(Replicated,BlueprintReadOnly)
	int32 IRedTeamPlayerCount=0;

	UPROPERTY(Replicated,BlueprintReadOnly)
	TArray<ATaskPlayerControllerGameplay*> BlueTeamPlayers;

	UPROPERTY(Replicated,BlueprintReadOnly)
	TArray<ATaskPlayerControllerGameplay*> RedTeamPlayers;



	
	UPROPERTY(Replicated,BlueprintReadOnly)
	int32 IBlueTeamKillScore=0;

	UPROPERTY(Replicated,BlueprintReadOnly)
	int32 IRedTeamKillScore=0;
	

	UPROPERTY(Replicated,BlueprintReadOnly)
	int32 IBlueTeamScore=0;

	UPROPERTY(Replicated,BlueprintReadOnly)
	int32 IRedTeamScore=0;

	TEnumAsByte<EPlayerTeam> WinnerTeam = EPlayerTeam::None;

	
	UPROPERTY(BlueprintReadWrite,EditDefaultsOnly,Replicated)
	float FGameTimeLeft=180;

	UPROPERTY(BlueprintReadWrite,EditDefaultsOnly,Replicated)
	float FTimeDecreaseInterval=1;

	FTimerHandle GameTimeDecreaseHandle;


	
	UFUNCTION(BlueprintCallable)
	void StartGameTimeCountdown();

	void AddScoreToTheTeam(TEnumAsByte<EPlayerTeam> Team);

	UPROPERTY(BlueprintAssignable)
	FOnTeamScoreChanged OnTeamScoreChanged;

	void DecreaseGameTime();

	UPROPERTY(BlueprintAssignable)
	FOnGameTimeChanged OnGameTimeChanged;

	void OnReceiveKill(ATaskPlayerStateGameplay* KillerPlayerState , ATaskPlayerStateGameplay* VictimPlayerState);

	UPROPERTY(BlueprintAssignable)
	FOnPlayerKill OnPlayerKill;

	UPROPERTY(BlueprintAssignable)
	FOnEndGame OnEndGame;

protected:

	UFUNCTION(NetMulticast,Reliable)
	void AddPlayerControllerToTeam(EPlayerTeam Team , ATaskPlayerControllerGameplay*NewPlayer);

	virtual void GetLifetimeReplicatedProps(TArray < FLifetimeProperty > & OutLifetimeProps) const override;
};
