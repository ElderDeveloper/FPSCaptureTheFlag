// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "FPSGameLibrary.generated.h"

/**
 * 
 */
UCLASS()
class CTFTASK_API UFPSGameLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	static  FSlateColor GetUIButtonTextColor(UWorld* World , FName ColorParameter);

	static FString BytesToStringFixed(const uint8* In, int32 Count);


	static FText IntToText(int32 i);

	
};
