// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "MAPCache.generated.h"

/**
 * 
 */
UCLASS()
class UNREALMAP_API UMAPCache : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(Category="MAPCache", VisibleAnywhere)
	TMap<FString, TObjectPtr<UTexture2D>> Textures;
	
	UPROPERTY(Category="MAPCache", VisibleAnywhere)
	TMap<FString, TObjectPtr<UMaterialInterface>> Materials;
};
