#pragma once

#include "Blueprint/UserWidget.h"


#include "SCKillNotifyWidget.generated.h"

class UTextBlock;
class ATaskPlayerStateGameplay;
UCLASS(BlueprintType,Blueprintable)
class USCKillNotifyWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	//aWeaponRef->AmmoStateChangedDelegate.AddDynamic(this,&USWidgetWeapon::AmmoStateChanged);

	UPROPERTY(BlueprintReadWrite,meta = (BindWidget))
	UTextBlock* T_Killer;

	UPROPERTY(BlueprintReadWrite,meta = (BindWidget))
	UTextBlock* T_Victim;

	FTimerHandle DestroyTimeHandle;

	UPROPERTY(EditDefaultsOnly)
	FSlateColor BlueTeamColor;
	
	UPROPERTY(EditDefaultsOnly)
	FSlateColor RedTeamColor;
	
	float FLifeTime=4;
	
	void SetupWidget(ATaskPlayerStateGameplay*PSKiller , ATaskPlayerStateGameplay*PSVictim);
	
};
