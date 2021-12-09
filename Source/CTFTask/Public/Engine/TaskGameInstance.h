// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "TaskGameInstance.generated.h"

/**
 * 
 */

#define SESSION_NAME TEXT("CaptureTheFlag")

class UMaterialParameterCollection;
UCLASS()
class CTFTASK_API UTaskGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
	UTaskGameInstance();
protected:
	UPROPERTY()
	UMaterialParameterCollection* MenuButtonMaterialCollection;



	TSharedPtr<class FOnlineSessionSearch> SessionSearch;
public:
	IOnlineSessionPtr SessionInterface;
	
	FORCEINLINE UMaterialParameterCollection*  GetButtonMaterialCollection() const  {	return MenuButtonMaterialCollection; }
	
	void FindSessions();

	void OnCreateSessionComplete (const FName SessionName,const bool Success);

	void OnDestroySessionComplete (const FName SessionName,const bool Success);

	void OnFindSessionsComplete (const bool Success);
	
	void CreateSession();
	
	virtual void Init() override;
};
