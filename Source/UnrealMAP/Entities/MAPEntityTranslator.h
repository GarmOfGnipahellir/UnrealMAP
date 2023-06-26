// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MAPEntityTypes.h"
#include "UObject/Object.h"
#include "MAPEntityTranslator.generated.h"

UCLASS(Blueprintable)
class UNREALMAP_API UMAPEntityTranslator : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(Category="MAPEntityTranslator", BlueprintNativeEvent)
	UClass* GetSupportedClass() const;

	UFUNCTION(Category="MAPEntityTranslator", BlueprintNativeEvent)
	TArray<FString> GetClassProperties(TSubclassOf<AActor> ActorClass) const;

	UFUNCTION(Category="MAPEntityTranslator", BlueprintNativeEvent)
	TArray<FString> GetPropertyPaths(TSubclassOf<AActor> ActorClass) const;

	FString ToFGD(TSubclassOf<AActor> ActorClass) const;
};
