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

	bool IsConfigValid(const UMAPConfig* Config) const;

	bool ExportGameConfig(const UMAPConfig* Config, const FString& OutputDir) const;
	bool ExportGameEngineProfiles(const UMAPConfig* Config, const FString& OutputDir) const;
	bool ExportEntitiesDefinition(const FString& OutputDir) const;

	bool ExportTextures(const UMAPConfig* Config) const;

protected:
	FString GetFilePath() const;
};
