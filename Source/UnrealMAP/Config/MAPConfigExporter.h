// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MAPConfig.h"
#include "Exporters/Exporter.h"
#include "MAPConfigExporter.generated.h"

class UMAPConfig;

UCLASS()
class UNREALMAP_API UMAPConfigExporter : public UExporter
{
	GENERATED_BODY()

public:
	UMAPConfigExporter();

	virtual bool ExportBinary(
		UObject* Object,
		const TCHAR* Type,
		FArchive& Ar,
		FFeedbackContext* Warn,
		int32 FileIndex = 0,
		uint32 PortFlags = 0) override;

	static bool IsConfigValid(const UMAPConfig* Config);

	static bool ExportGameConfig(const UMAPConfig* Config, const FString& OutputDir);
	static bool ExportGameEngineProfiles(const UMAPConfig* Config, const FString& OutputDir);

	static FString ActorClassToEntityDefinition(const TSubclassOf<AActor> ActorClass);
	static bool ExportEntitiesDefinition(const FString& OutputDir);

	static bool ExportTextures(const UMAPConfig* Config);
	static FString GetAssetTexturePath(const UMAPConfig* Config, const FString& ObjectPath);
	static bool ExportTextureAsset(const UMAPConfig* Config, const FAssetData& Asset);

protected:
	FString GetFilePath() const;
};
