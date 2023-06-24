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
	EMAPClassType GetClassType(TSubclassOf<AActor> ActorClass) const;

	UFUNCTION(Category="MAPEntityTranslator", BlueprintNativeEvent)
	TArray<FMAPClassProperty> GetClassProps(TSubclassOf<AActor> ActorClass) const;

	UFUNCTION(Category="MAPEntityTranslator", BlueprintNativeEvent)
	FString GetEntityName(TSubclassOf<AActor> ActorClass) const;

	UFUNCTION(Category="MAPEntityTranslator", BlueprintNativeEvent)
	TArray<FMAPEntityProperty> GetEntityProps(TSubclassOf<AActor> ActorClass) const;

	UFUNCTION(Category="MAPEntityTranslator", BlueprintNativeEvent)
	TArray<FString> GetPropNames(TSubclassOf<AActor> ActorClass) const;

	FString ToFGD(TSubclassOf<AActor> ActorClass) const;

	static FString ClassTypeToString(const EMAPClassType& ClassType);
	static FString ClassPropToString(const FMAPClassProperty& ClassProp);
	static FString ClassPropsToString(const TArray<FMAPClassProperty>& ClassProps);
	static FString EntityPropTypeToString(const EMAPEntityPropertyType& EntityPropType);
	static FString EntityPropToString(const FMAPEntityProperty& EntityProp);
	static FString EntityPropsToString(const TArray<FMAPEntityProperty>& EntityProps);

	static FPropertyRef GetProperty(const FString& PropertyName, TSubclassOf<AActor> ActorClass);
	static FString PropertyToFGD(const FString& PropertyName, const FPropertyRef& PropertyRef);
};
