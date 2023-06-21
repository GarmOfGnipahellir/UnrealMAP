// Copyright Epic Games, Inc. All Rights Reserved.

#include "UnrealMAP.h"

#include "ISettingsModule.h"
#include "MAPSettings.h"

#define LOCTEXT_NAMESPACE "FUnrealMAPModule"

void FUnrealMAPModule::StartupModule()
{
	if (const auto SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		SettingsModule->RegisterSettings(
			"Project",
			"Plugins",
			"Unreal MAP",
			LOCTEXT("UnrealMAPSettingsCategoryDisplayName", "Unreal MAP"),
			LOCTEXT("UnrealMAPSettingsDescription", "Configure the Unreal MAP project settings"),
			GetMutableDefault<UMAPSettings>()
		);
	}
}

void FUnrealMAPModule::ShutdownModule()
{
	if (const auto SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		SettingsModule->UnregisterSettings("Project", "Plugins", "Unreal MAP");
	}
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FUnrealMAPModule, UnrealMAP)
