#pragma once
#include "GameFramework/GameStateBase.h"
#include "SGameplayData.h"

#include "TaskGameStateGameplay.generated.h"

class ATaskPlayerControllerGameplay;
class ATaskPlayerStateGameplay;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnTeamScoreChanged,int32,IBlueTeam,int32,IRedTeam);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGameTimeChanged,float,FGameTime);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnPlayerKill,ATaskPlayerStateGameplay*,KillerState,ATaskPlayerStateGameplay*,VictimState);


UCLASS()
class ATaskGameStateGameplay : public AGameStateBase
{
public:

	GENERATED_BODY()
	
	ATaskGameStateGameplay();
	
	void Server_AddNewPlayerToTheTeam(TEnumAsByte<EPlayerTeam> PlayerT , class ATaskPlayerControllerGameplay* PlayerC);
	
	UPROPERTY(Replicated)
	int32 IBlueTeamPlayerCount=0;

	UPROPERTY(Replicated)
	int32 IRedTeamPlayerCount=0;

	UPROPERTY(Replicated)
	TArray<ATaskPlayerControllerGameplay*> BlueTeamPlayers;

	UPROPERTY(Replicated)
	TArray<ATaskPlayerControllerGameplay*> RedTeamPlayers;



	
	UPROPERTY(Replicated)
	int32 IBlueTeamKillScore=0;

	UPROPERTY(Replicated)
	int32 IRedTeamKillScore=0;
	

	UPROPERTY(Replicated)
	int32 IBlueTeamScore=0;

	UPROPERTY(Replicated)
	int32 IRedTeamScore=0;



	
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

	

	virtual void GetLifetimeReplicatedProps(TArray < FLifetimeProperty > & OutLifetimeProps) const override;
};
