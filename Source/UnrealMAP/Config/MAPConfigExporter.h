// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Exporters/Exporter.h"
#include "MAPConfigExporter.generated.h"

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

protected:
	FString GetFilePath() const;
};
