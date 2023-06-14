// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MAPStage.generated.h"

UCLASS()
class UNREALMAP_API AMAPStage : public AActor
{
	GENERATED_BODY()

public:
	FFilePath SourceFile;
	
	AMAPStage();
};
