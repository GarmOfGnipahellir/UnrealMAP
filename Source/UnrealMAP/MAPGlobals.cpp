// Fill out your copyright notice in the Description page of Project Settings.


#include "MAPGlobals.h"

#include "MAPSettings.h"

UMAPConfig* UMAPGlobals::GetDefaultConfig()
{
	return GetDefault<UMAPSettings>()->DefaultConfig.Get();
}

bool UMAPGlobals::ExportTrenchBroomConfig()
{
	return false;
}
