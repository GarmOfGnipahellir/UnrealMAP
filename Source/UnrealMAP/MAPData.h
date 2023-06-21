// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MAPData.generated.h"

USTRUCT(BlueprintType)
struct UNREALMAP_API FMAPData
{
	GENERATED_BODY()

	UPROPERTY(Category="MAPData", EditAnywhere)
	FDirectoryPath TextureRoot;

	UPROPERTY(Category="MAPData", EditAnywhere)
	TArray<FString> TextureCollections;

	UPROPERTY(Category="MAPData", EditAnywhere)
	TObjectPtr<UMaterialInterface> DefaultMaterial;
};
