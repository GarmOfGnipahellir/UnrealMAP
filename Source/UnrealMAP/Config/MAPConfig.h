// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MAPConfig.generated.h"

UCLASS()
class UNREALMAP_API UMAPConfig : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(Category="MAPConfig", EditAnywhere, meta=(ContentDir))
	FDirectoryPath TextureRoot;

	UPROPERTY(Category="MAPConfig", EditAnywhere)
	TObjectPtr<UMaterialInterface> DefaultMaterial;

	FString GetTextureRootPlatform() const;

	virtual FName GetExporterName() override;
};
