// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "MAPSettings.generated.h"

/**
 * 
 */
UCLASS(Config="UnrealMAP")
class UNREALMAP_API UMAPSettings : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(Category="Textures", Config, EditAnywhere, meta=(ContentDir))
	TArray<FDirectoryPath> TextureSources;
};
