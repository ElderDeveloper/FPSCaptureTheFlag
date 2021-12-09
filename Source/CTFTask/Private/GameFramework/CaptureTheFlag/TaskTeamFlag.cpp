#include "GameFramework/CaptureTheFlag/TaskTeamFlag.h"

#include "GeneratedCodeHelpers.h"
#include "Components/CapsuleComponent.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/CaptureTheFlag/CaptureTheFlagInterface.h"
#include "GameFramework/CaptureTheFlag/TaskTeamFlagBase.h"

ATaskTeamFlag::ATaskTeamFlag()
{
	RootScene=CreateDefaultSubobject<USceneComponent>(TEXT("RootScene"));
	RootComponent=RootScene;
	
	FlagMesh=CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FlagMesh"));
	FlagMesh->SetupAttachment(RootScene);
	FlagMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	FlagCollision=CreateDefaultSubobject<UCapsuleComponent>(TEXT("FlagCollision"));
	FlagCollision->SetupAttachment(RootScene);
	FlagCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	FlagCollision->SetCollisionResponseToAllChannels(ECR_Overlap);

}

void ATaskTeamFlag::SetupFlagTeam(TEnumAsByte<EPlayerTeam> Team , ATaskTeamFlagBase*Base)
{
	if (Base)
	{
		FlagBase=Base;
	}
	FlagTeam=Team;

}


void ATaskTeamFlag::OnFlagDrop()
{
	FlagCollision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}


void ATaskTeamFlag::OnFlagTakenFromGround(AActor* EnemyActor)
{
	ICaptureTheFlagInterface*TeamInterface = Cast<ICaptureTheFlagInterface>(EnemyActor);
	if (TeamInterface)
	{
		TeamInterface->ReceiveEnemyFlag(this);
	}
}


void ATaskTeamFlag::OnFlagReturnToBase()
{
	if (FlagBase)
	{
		SetActorLocationAndRotation(FlagBase->FlagSpawnPoint->GetComponentLocation(),FRotator::ZeroRotator);
		FlagBase->bFlagIsInBase=true;
		FlagCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void ATaskTeamFlag::OnCapsuleOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ICaptureTheFlagInterface*TeamInterface = Cast<ICaptureTheFlagInterface>(OtherActor);
	if (TeamInterface)
	{
		const EPlayerTeam ReceivedTeam=TeamInterface->GetPlayerTeam();
		
		if (ReceivedTeam!=None)
		{
			if (FlagTeam==ReceivedTeam)
			{
				OnFlagReturnToBase();
			}
			else
			{
				TeamInterface->ReceiveEnemyFlag(this);
			}

		}
		
	}
}

void ATaskTeamFlag::BeginPlay()
{
	Super::BeginPlay();
	FlagCollision->OnComponentBeginOverlap.AddDynamic(this,&ATaskTeamFlag::OnCapsuleOverlap);
}


void ATaskTeamFlag::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ATaskTeamFlag,FlagTeam);
	DOREPLIFETIME(ATaskTeamFlag,FlagBase);
}
