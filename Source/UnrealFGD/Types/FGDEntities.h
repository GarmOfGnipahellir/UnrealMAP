// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "FGDEntities.generated.h"

class UFGDEntity;

UCLASS()
class UNREALFGD_API UFGDEntities : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY()
	TMap<FString, TObjectPtr<UFGDEntity>> Entities;

	UFGDEntity* GetEntityFromObjectClass(const UClass* InClass) const;
};
