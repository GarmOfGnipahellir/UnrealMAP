// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "FGDProperty.generated.h"

UCLASS()
class UNREALFGD_API UFGDProperty : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(Category="FGDProperty", EditAnywhere)
	FString Name;

	UPROPERTY(Category="FGDProperty", EditAnywhere)
	FString Path;

	UPROPERTY(Category="FGDProperty", EditAnywhere)
	FString Type;

	UPROPERTY(Category="FGDProperty", EditAnywhere)
	FString Default;

	UPROPERTY(Category="FGDProperty", EditAnywhere)
	FString Description;

	static UFGDProperty* CreateFromClass(const FString& InPath, const UClass* InClass);

	void SetOnObject(const FString& InValue, UObject* InObject) const;

protected:
	static UObject* GetContainerFromPath(const FString& InPath, UObject* InObject);
};
