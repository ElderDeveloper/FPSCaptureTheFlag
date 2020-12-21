#pragma once

#include "Blueprint/UserWidget.h"
#include "SCKillNotifyWidget.generated.h"

class UTextBlock;
class UImage;
class USizeBox;

UCLASS(BlueprintType,Blueprintable)
class USCKillNotifyWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	//aWeaponRef->AmmoStateChangedDelegate.AddDynamic(this,&USWidgetWeapon::AmmoStateChanged);

	UPROPERTY(BlueprintReadWrite,meta = (BindWidget))
	class UImage* AimImage_01;
	
};
