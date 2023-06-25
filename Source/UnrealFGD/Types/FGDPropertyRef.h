// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "FGDPropertyRef.generated.h"

/**
 * 
 */
UCLASS()
class UNREALFGD_API UFGDPropertyRef : public UObject
{
	GENERATED_BODY()

public:
	FProperty* Property;
	UObject* Container;
};
