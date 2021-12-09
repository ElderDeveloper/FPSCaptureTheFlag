// Fill out your copyright notice in the Description page of Project Settings.


#include "Engine/TaskGameInstance.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include "Kismet/GameplayStatics.h"
#include "Materials/MaterialParameterCollection.h"

UTaskGameInstance::UTaskGameInstance()
{
	const ConstructorHelpers::FObjectFinder<UMaterialParameterCollection> MPCollectionConstruct (TEXT("/Game/UI/Global/MC_ButtonTextColor"));
	MenuButtonMaterialCollection=MPCollectionConstruct.Object;
}





void UTaskGameInstance::FindSessions()
{
	SessionSearch=MakeShareable(new FOnlineSessionSearch());
	if (SessionSearch.IsValid())
	{
		SessionSearch->bIsLanQuery=true;
		SessionInterface->FindSessions(0,SessionSearch.ToSharedRef());
	}
}


void UTaskGameInstance::OnCreateSessionComplete(const FName SessionName,const bool Success)
{
	if (!Success)
	{
		return;
	}
	UGameplayStatics::OpenLevel(GetWorld(),"Lobby",true,"listen");
}


void UTaskGameInstance::OnDestroySessionComplete(const FName SessionName, const bool Success)
{
	if (Success)
	{
		CreateSession();
	}
}


void UTaskGameInstance::OnFindSessionsComplete(const bool Success)
{

	if (Success && SessionSearch.IsValid())
	{
		for (const auto& Result : SessionSearch->SearchResults)
		{
			if (Result.IsValid())
			{

				SessionInterface->JoinSession(0,SESSION_NAME,Result);
				return;
			}
		}
	}

	CreateSession();
}


void UTaskGameInstance::CreateSession()
{
	if(SessionInterface.IsValid())
	{
		FOnlineSessionSettings SessionSettings;
		SessionSettings.bIsLANMatch=true;
		SessionSettings.NumPublicConnections=8;
		SessionSettings.bShouldAdvertise=true;
		SessionInterface->CreateSession(0,SESSION_NAME , SessionSettings);
	}

}


void UTaskGameInstance::Init()
{

	IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
	
	if(Subsystem!=nullptr)
	{
		SessionInterface = Subsystem->GetSessionInterface();
		
		if (SessionInterface.IsValid())
		{
			SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this,&UTaskGameInstance::OnCreateSessionComplete);
			SessionInterface->OnDestroySessionCompleteDelegates.AddUObject(this,&UTaskGameInstance::OnDestroySessionComplete);
			SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this,&UTaskGameInstance::OnFindSessionsComplete);
		}
		
	}
}


