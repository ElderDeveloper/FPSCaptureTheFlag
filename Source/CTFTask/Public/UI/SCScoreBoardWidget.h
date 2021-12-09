#pragma once

#include "Blueprint/UserWidget.h"
#include "SCScoreBoardWidget.generated.h"


class UTextBlock;
class UImage;
UCLASS(BlueprintType,Blueprintable)
class USCScoreBoardWidget : public UUserWidget
{

	GENERATED_BODY()
public:

	UPROPERTY(BlueprintReadWrite,meta = (BindWidget))
	UImage*Img_DeathState;

	UPROPERTY(BlueprintReadWrite,meta = (BindWidget))
	UTextBlock*T_PlayerName;

	UPROPERTY(BlueprintReadWrite,meta = (BindWidget))
	UTextBlock*T_PlayerKillCount;

	UPROPERTY(BlueprintReadWrite,meta = (BindWidget))
	UTextBlock*T_Ping;


	UPROPERTY(EditDefaultsOnly)
	FSlateColor BlueTeamColor;

	UPROPERTY(EditDefaultsOnly)
	FSlateColor RedTeamColor;
	
	void SetupScoreBoardChildWidget(class ATaskPlayerStateGameplay* PSOwner);

	
};
