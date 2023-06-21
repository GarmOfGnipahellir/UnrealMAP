// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MAPConfig.generated.h"

UCLASS()
class UNREALMAP_API UMAPConfig : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(Category="Textures", EditAnywhere, meta=(ContentDir))
	FDirectoryPath TextureRoot;

	UFUNCTION(Category="Export", CallInEditor)
	void Export();

	FString GetTextureRootPlatform() const;

	bool ExportForTrenchBroom(const FString& OutputDir) const;

	virtual FName GetExporterName() override;
};
