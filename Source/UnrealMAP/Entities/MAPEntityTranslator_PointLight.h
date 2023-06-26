// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MAPEntityTranslator.h"
#include "UObject/Object.h"
#include "MAPEntityTranslator_PointLight.generated.h"

/**
 * 
 */
UCLASS()
class UNREALMAP_API UMAPEntityTranslator_PointLight : public UMAPEntityTranslator
{
	GENERATED_BODY()

	virtual UClass* GetSupportedClass_Implementation() const override;
	virtual TArray<FString> GetClassProperties_Implementation(TSubclassOf<AActor> ActorClass) const override;
	virtual TArray<FString> GetPropertyPaths_Implementation(TSubclassOf<AActor> ActorClass) const override;
};
