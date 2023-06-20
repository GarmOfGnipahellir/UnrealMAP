// Copyright Epic Games, Inc. All Rights Reserved.

#include "UnrealMAPEditor.h"

#include "MAPComponentVisualizer.h"
#include "UnrealEdGlobals.h"
#include "Editor/UnrealEdEngine.h"
#include "UnrealMAP/MAPComponent.h"

#define LOCTEXT_NAMESPACE "FUnrealMAPEditorModule"

void FUnrealMAPEditorModule::StartupModule()
{
	if (GUnrealEd != nullptr)
	{
		if (const TSharedPtr<FMAPComponentVisualizer> Visualizer = MakeShareable(
			new FMAPComponentVisualizer
		); Visualizer.IsValid())
		{
			GUnrealEd->RegisterComponentVisualizer(UMAPComponent::StaticClass()->GetFName(), Visualizer);
			Visualizer->OnRegister();
		}
	}
}

void FUnrealMAPEditorModule::ShutdownModule()
{
	if (GUnrealEd != nullptr)
	{
		GUnrealEd->UnregisterComponentVisualizer(UMAPComponent::StaticClass()->GetFName());
	}
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FUnrealMAPEditorModule, UnrealMAPEditor)
