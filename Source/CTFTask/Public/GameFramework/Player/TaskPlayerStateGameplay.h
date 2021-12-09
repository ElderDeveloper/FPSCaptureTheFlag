#pragma once
#include "GameFramework/PlayerState.h"
#include "SGameplayData.h"
#include "TaskPlayerStateGameplay.generated.h"

UCLASS()
class ATaskPlayerStateGameplay : public APlayerState
{
	GENERATED_BODY()
public:

	ATaskPlayerStateGameplay();
	
	UPROPERTY(Replicated,BlueprintReadOnly)
	TEnumAsByte<EPlayerTeam> PlayerTeam;

	int32 IPlayerKillCount;

	bool bIsAlive=true;

	virtual void GetLifetimeReplicatedProps(TArray < FLifetimeProperty > & OutLifetimeProps) const override;
};
