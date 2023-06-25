// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

namespace FGDUtils
{
	FString PascalCaseToSnakeCase(const FString& InString);

	TOptional<double> ParseDouble(const FString& InString);
	TOptional<int> ParseInteger(const FString& InString);
	TOptional<FVector> ParseVector(const FString& InString);
	TOptional<FColor> ParseColor(const FString& InString);
}
