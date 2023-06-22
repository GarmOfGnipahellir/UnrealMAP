// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
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

	bool ExportGameConfig(const UMAPConfig* Config, const FString& OutputDir) const;
	bool ExportGameEngineProfiles(const FString& OutputDir) const;
	bool ExportEntitiesDefinition(const FString& OutputDir) const;

protected:
	FString GetFilePath() const;
};
