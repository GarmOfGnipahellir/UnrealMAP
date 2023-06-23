// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "MAPGlobals.generated.h"

class UMAPConfig;

UCLASS()
class UNREALMAP_API UMAPGlobals : public UObject
{
	GENERATED_BODY()

public:
	static UMAPConfig* GetDefaultConfig();
};
