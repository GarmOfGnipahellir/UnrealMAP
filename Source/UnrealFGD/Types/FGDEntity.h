// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "FGDEntity.generated.h"

class UFGDProperty;

UCLASS()
class UNREALFGD_API UFGDEntity : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY()
	TMap<FString, TObjectPtr<UFGDProperty>> Properties;

	static UFGDEntity* CreateFromClass(const TArray<FString>& InPaths, const UClass* InClass);

	UFGDProperty* GetPropertyFromName(const FString& InName) const;

	void SetPropertiesOnObject(const TMap<FString, FString>& InProperties, UObject* InObject) const;
};
