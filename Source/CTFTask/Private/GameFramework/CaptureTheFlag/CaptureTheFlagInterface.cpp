// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFramework/CaptureTheFlag/CaptureTheFlagInterface.h"

// Add default functionality here for any ICaptureTheFlagInterface functions that are not pure virtual.
EPlayerTeam ICaptureTheFlagInterface::GetHasFlagAndTeam()
{
	return  None;
}

EPlayerTeam ICaptureTheFlagInterface::GetPlayerTeam()
{
	return None;
}

ATaskTeamFlag* ICaptureTheFlagInterface::GetTeamFlag()
{
	return nullptr;
}

void ICaptureTheFlagInterface::ReceiveFlagCarriedToStation()
{
}

void ICaptureTheFlagInterface::ReceiveEnemyFlag(ATaskTeamFlag* FlagRef)
{
}
