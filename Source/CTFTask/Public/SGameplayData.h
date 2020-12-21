#pragma once

#include "SGameplayData.generated.h"


UENUM(Blueprintable,BlueprintType)
enum EPlayerTeam
{
	None		UMETA(DisplayName ="None"),
	BlueTeam	UMETA(DisplayName ="BlueTeam"),
	RedTeam		UMETA(DisplayName ="RedTeam")
};



UCLASS()
class USGameplayData : public UObject
{
	GENERATED_BODY()
public:
	
};
