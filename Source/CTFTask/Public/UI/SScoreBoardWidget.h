#pragma once

#include "Blueprint/UserWidget.h"
#include "SGameplayData.h"
#include "SScoreBoardWidget.generated.h"

class UTextBlock;
class UVerticalBox;
class USCScoreBoardWidget;
class ATaskPlayerStateGameplay;

UCLASS(BlueprintType,Blueprintable)
class USScoreBoardWidget : public UUserWidget
{

	GENERATED_BODY()
public:

	UPROPERTY(BlueprintReadWrite,meta = (BindWidget))
	UTextBlock*T_BlueTeamTotalScore;
		
	UPROPERTY(BlueprintReadWrite,meta = (BindWidget))
	UTextBlock*T_RedTeamTotalScore;

	UPROPERTY(BlueprintReadWrite,meta = (BindWidget))
	UVerticalBox*VB_BlueTeam;

	UPROPERTY(BlueprintReadWrite,meta = (BindWidget))
	UVerticalBox*VB_RedTeam;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<USCScoreBoardWidget> SBChildClass;
	
    UPROPERTY()
	class ATaskGameStateGameplay*GSGameplay;
	
	void OnScoresReceiveUpdate(int32 IBlueTeam,int32 IRedTeam);

	void OnKillScoreReceiveUpdate(ATaskPlayerStateGameplay*KillerState,ATaskPlayerStateGameplay* VictimState);

	void FillScoreBoardChilds();
	
	virtual void NativeConstruct() override;

	virtual bool Initialize() override;
	
};
