#include "GameFramework/CaptureTheFlag/TaskTeamFlagBase.h"

#include "GeneratedCodeHelpers.h"
#include "Components/CapsuleComponent.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/CaptureTheFlag/CaptureTheFlagInterface.h"
#include "GameFramework/CaptureTheFlag/TaskTeamFlag.h"
#include "GameFramework/Core/TaskGameStateGameplay.h"


ATaskTeamFlagBase::ATaskTeamFlagBase()
{
	RootScene=CreateDefaultSubobject<USceneComponent>(TEXT("RootScene"));
	RootComponent=RootScene;

	FlagSpawnPoint=CreateDefaultSubobject<USceneComponent>(TEXT("FlagSpawnPoint"));
	FlagSpawnPoint->SetupAttachment(RootScene);

	FlagAreaMesh=CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FlagAreaMesh"));
	FlagAreaMesh->SetupAttachment(RootScene);
	FlagAreaMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	FlagCollision=CreateDefaultSubobject<UCapsuleComponent>(TEXT("FlagCollision"));
	FlagCollision->SetupAttachment(RootScene);
	FlagCollision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	FlagCollision->SetCollisionResponseToAllChannels(ECR_Overlap);
	
}


void ATaskTeamFlagBase::OnCapsuleOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		ICaptureTheFlagInterface* FlagInterface = Cast<ICaptureTheFlagInterface>(OtherActor);

		if(FlagInterface)
		{
			if (FlagInterface->GetPlayerTeam()!=FlagBaseTeam && bFlagIsInBase && FlagInterface->GetHasFlagAndTeam() == None)
			{
				FlagInterface->ReceiveEnemyFlag(TeamFlag);
				
				bFlagIsInBase=false;
			}

			if (FlagInterface->GetPlayerTeam()==FlagBaseTeam && bFlagIsInBase && FlagInterface->GetHasFlagAndTeam()!=None)
			{
				ATaskTeamFlag* FlagRef=FlagInterface->GetTeamFlag();
				if(FlagRef)
				{
					FlagInterface->ReceiveFlagCarriedToStation();
					
					FlagRef->OnFlagReturnToBase();
					
					ATaskGameStateGameplay* GS=Cast<ATaskGameStateGameplay>(GetWorld()->GetGameState());
					if(GS)
					{
						GS->AddScoreToTheTeam(FlagBaseTeam);
					}
				}
				
			}
			
		}
		
	}

}

void ATaskTeamFlagBase::Server_SpawnTeamFlag_Implementation()
{
	FActorSpawnParameters SpawnParams;
	ATaskTeamFlag*Flag = GetWorld()->SpawnActor<ATaskTeamFlag>(TeamFlagClass,FlagSpawnPoint->GetComponentLocation(),FlagSpawnPoint->GetComponentRotation(),SpawnParams);
	if(Flag)
	{
		Multi_FlagSpawned(Flag);
	}
	ForceNetUpdate();
}

void ATaskTeamFlagBase::Multi_FlagSpawned_Implementation(ATaskTeamFlag* Ref)
{
	TeamFlag=Ref;
	bFlagIsInBase=true;
	TeamFlag->SetupFlagTeam(FlagBaseTeam,this);
}

void ATaskTeamFlagBase::EnemyFlagCaptured()
{

}

void ATaskTeamFlagBase::ReturnFlagToBase()
{
	bFlagIsInBase=true;
}

void ATaskTeamFlagBase::TeamLostFlag()
{
	bFlagIsInBase=false;
}

void ATaskTeamFlagBase::BeginPlay()
{
	Super::BeginPlay();
	FlagCollision->OnComponentBeginOverlap.AddDynamic(this,&ATaskTeamFlagBase::OnCapsuleOverlap);
	Server_SpawnTeamFlag();
	
}

void ATaskTeamFlagBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ATaskTeamFlagBase,TeamFlag);
	DOREPLIFETIME(ATaskTeamFlagBase,bFlagIsInBase);
}
