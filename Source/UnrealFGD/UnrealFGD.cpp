// Copyright Epic Games, Inc. All Rights Reserved.

#include "UnrealFGD.h"

#define LOCTEXT_NAMESPACE "FUnrealFGDModule"

FUnrealFGDModule* FUnrealFGDModule::Get()
{
	return FModuleManager::GetModulePtr<FUnrealFGDModule>("UnrealFGD");
}

void FUnrealFGDModule::StartupModule() {}

void FUnrealFGDModule::ShutdownModule() {}

UFGDEntity* FUnrealFGDModule::ObjectClassToEntityDefinition(UClass* InClass)
{
	return nullptr;
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FUnrealFGDModule, UnrealFGD)
