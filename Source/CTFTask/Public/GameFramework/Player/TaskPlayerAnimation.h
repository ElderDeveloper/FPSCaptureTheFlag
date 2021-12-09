#pragma once


#include "TaskPlayerAnimation.generated.h"

UCLASS()
class UTaskPlayerAnimation : public UAnimInstance
{
	GENERATED_BODY()
public:

	UPROPERTY(BlueprintReadOnly)
	float FLookUpAlpha;

	UPROPERTY(BlueprintReadOnly)
	float FPlayerSpeed;

	UPROPERTY(BlueprintReadOnly)
	class ACTFTaskCharacter*Player=nullptr;

	virtual void NativeInitializeAnimation() override;
	
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
};
