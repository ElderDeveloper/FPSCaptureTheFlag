#include "GameFramework/Player/TaskPlayerStateGameplay.h"

#include "GeneratedCodeHelpers.h"

ATaskPlayerStateGameplay::ATaskPlayerStateGameplay()
{
}

void ATaskPlayerStateGameplay::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ATaskPlayerStateGameplay,PlayerTeam);
}
