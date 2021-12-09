#include "GameFramework/CaptureTheFlag/TaskTeamPlayerStart.h"

#include "GameFramework/Core/TaskGameModeGameplay.h"



void ATaskTeamPlayerStart::Server_SendReferanceToGameMode_Implementation()
{
	ATaskGameModeGameplay*GM=Cast<ATaskGameModeGameplay>(GetWorld()->GetAuthGameMode());
	if(GM)
	{
		GM->PlayerStarts.AddUnique(this);
	}
}



void ATaskTeamPlayerStart::BeginPlay()
{
	Super::BeginPlay();

	if (GetLocalRole()==ROLE_Authority)
	{
		Server_SendReferanceToGameMode();
	}
	
}
