// Fill out your copyright notice in the Description page of Project Settings.


#include "Library/FPSGameLibrary.h"

#include "Materials/MaterialParameterCollection.h"
#include "Engine/TaskGameInstance.h"

FSlateColor UFPSGameLibrary::GetUIButtonTextColor(UWorld* World, FName ColorParameter)
{
	if(World)
	{
		UTaskGameInstance*GI=Cast<UTaskGameInstance>(World->GetGameInstance());
		if(GI)	return GI->GetButtonMaterialCollection()->GetVectorParameterByName(ColorParameter)->DefaultValue;
	}
	
	return FSlateColor(FLinearColor(FColor::White));
}

FString UFPSGameLibrary::BytesToStringFixed(const uint8* In, int32 Count)
{
	FString Broken = BytesToString(In, Count);
	FString Fixed;

	for (int i = 0; i < Broken.Len(); i++)
	{
		const TCHAR c = Broken[i] - 1;
		Fixed.AppendChar(c);
	}

	return Fixed;
}

FText UFPSGameLibrary::IntToText(int32 i)
{
	return FText::FromString(FString::FromInt(i));
}
