// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class UFGDEntity;

class FUnrealFGDModule : public IModuleInterface
{
public:
	static FUnrealFGDModule* Get();
	
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

	UFGDEntity* ObjectClassToEntityDefinition(UClass* InClass);
};
