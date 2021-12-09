#pragma once

#include "SGameplayData.generated.h"


UENUM(Blueprintable,BlueprintType)
enum EPlayerTeam
{
	None		UMETA(DisplayName ="None"),
	BlueTeam	UMETA(DisplayName ="BlueTeam"),
	RedTeam		UMETA(DisplayName ="RedTeam")
};

USTRUCT(BlueprintType)
struct FPlayersStruct
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite)
	FName PlayerName;

	UPROPERTY(BlueprintReadWrite)
	bool PlayerReady;
	FPlayersStruct(FName PN , bool PR)
	{
		PlayerName=PN;
		PlayerReady=PR;
	}
	FPlayersStruct(){}
};


UCLASS()
class USGameplayData : public UObject
{
	GENERATED_BODY()
public:
	
};
