#include "World/TaskTeamFlag.h"

#include "GeneratedCodeHelpers.h"
#include "Components/CapsuleComponent.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "World/TaskTeamFlagBase.h"

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
	if (FlagTeam!=None)
	{
		switch (FlagTeam)
		{
			case BlueTeam:
				FlagMesh->SetMaterial(0,BlueFlagMaterial);
				break;

			case RedTeam:
				FlagMesh->SetMaterial(0,RedFlagMaterial);
				break;

			default:
			break;	
		}
		
	}
}


void ATaskTeamFlag::OnFlagDrop()
{
	
}


void ATaskTeamFlag::OnFlagTakenFromGround()
{
}


void ATaskTeamFlag::OnFlagReturnToBase()
{
}

void ATaskTeamFlag::OnCapsuleOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
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
