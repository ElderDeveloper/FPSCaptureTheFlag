// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFramework/Core/TaskGameModeGameplay.h"

#include "GeneratedCodeHelpers.h"
#include "GameFramework/Player/TaskPlayerControllerGameplay.h"
#include "GameFramework/Core/TaskGameStateGameplay.h"
#include "GameFramework/CaptureTheFlag/TaskTeamPlayerStart.h"
#include "GameFramework/Player/TaskPlayerStateGameplay.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Player/TaskCharacter.h"

ATaskGameModeGameplay::ATaskGameModeGameplay() 
{

}


void ATaskGameModeGameplay::ReceiveTeamSelect(ATaskPlayerControllerGameplay* PlayerController)
{
	if (GPGameState!=nullptr && PlayerController!=nullptr)
	{
		const int32 team1=GPGameState->IBlueTeamPlayerCount;
		const int32 team2=GPGameState->IRedTeamPlayerCount;
		if(team1==team2)
		{
			const int32 RandomizeTeam = UKismetMathLibrary::RandomIntegerInRange(0,1);
			
			if (RandomizeTeam==1)
			{
				PlayerController->TeamSelectReceived(RedTeam);
				return;
			}
			
			PlayerController->TeamSelectReceived(BlueTeam);
			return;
			
		}
		if(team1>team2)
		{
			PlayerController->TeamSelectReceived(RedTeam);
			return;
		}
		PlayerController->TeamSelectReceived(BlueTeam);
		
	}
}


void ATaskGameModeGameplay::ReceiveSpawnPlayer(ATaskPlayerControllerGameplay * PlayerController , TEnumAsByte<EPlayerTeam> Team)
{
	if(Team!=None)
	{
		FTransform PlayerSpawnTransform;
		if(FindPlayerSpawnPoints(Team,PlayerSpawnTransform))
		{
			FActorSpawnParameters Parameters;
			Parameters.SpawnCollisionHandlingOverride=ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
			ACTFTaskCharacter* SpawnedCharacter=GetWorld()->SpawnActor<ACTFTaskCharacter>(PlayerGameplayClass,PlayerSpawnTransform.GetLocation(),PlayerSpawnTransform.GetRotation().Rotator(),Parameters);
			if(SpawnedCharacter)
			{
				if (PlayerController->GetPawn())
				{
					PlayerController->GetPawn()->Destroy();
				}
				PlayerController->Possess(SpawnedCharacter);
				SpawnedCharacter->PlayerTeam=Team;
			}
		}
	}
}


bool ATaskGameModeGameplay::FindPlayerSpawnPoints(TEnumAsByte<EPlayerTeam> Team, FTransform& Transform)
{
	TArray<ATaskTeamPlayerStart*> LocalPlayerStarts;
	
	for(auto i : PlayerStarts)
	{
		if(i->PlayerStartTeam == Team)
		{
			LocalPlayerStarts.AddUnique(i);
		}
	}
	
	if(LocalPlayerStarts.IsValidIndex(0))
	{
		int32 index = UKismetMathLibrary::RandomIntegerInRange(0,LocalPlayerStarts.Num()-1);

		if(LocalPlayerStarts.IsValidIndex(index))
		{
			Transform=LocalPlayerStarts[index]->GetActorTransform();
			return true;
		}
			
		return false;
	}		
	
	return false;
}


void ATaskGameModeGameplay::BeginPlay()
{
	Super::BeginPlay();

	GPGameState= Cast<ATaskGameStateGameplay>(GetWorld()->GetGameState());

}


void ATaskGameModeGameplay::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const 
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ATaskGameModeGameplay,PlayerStarts);
}
