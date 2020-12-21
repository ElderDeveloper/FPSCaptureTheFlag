#include "World/TaskTeamFlagBase.h"

#include "GeneratedCodeHelpers.h"
#include "Components/CapsuleComponent.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "World/TaskTeamFlag.h"


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
}

void ATaskTeamFlagBase::EnemyFlagCaptured()
{
}

void ATaskTeamFlagBase::ReturnFlagToBase()
{
}

void ATaskTeamFlagBase::TeamLostFlag()
{
}

void ATaskTeamFlagBase::BeginPlay()
{
	Super::BeginPlay();
	FlagCollision->OnComponentBeginOverlap.AddDynamic(this,&ATaskTeamFlagBase::OnCapsuleOverlap);
}

void ATaskTeamFlagBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ATaskTeamFlagBase,TeamFlag);
	DOREPLIFETIME(ATaskTeamFlagBase,bFlagIsInBase);
}
