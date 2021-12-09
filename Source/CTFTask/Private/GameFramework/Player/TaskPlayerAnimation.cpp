#include "GameFramework/Player/TaskPlayerAnimation.h"
#include "GameFramework/Player/TaskCharacter.h"

void UTaskPlayerAnimation::NativeInitializeAnimation()
{
	Player = Cast<ACTFTaskCharacter>(TryGetPawnOwner());
}

void UTaskPlayerAnimation::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (Player)
	{
		FLookUpAlpha=Player->FLookUpAlpha;

		FPlayerSpeed=Player->GetVelocity().Size();
	}
}
