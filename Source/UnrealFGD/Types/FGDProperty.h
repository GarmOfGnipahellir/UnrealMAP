// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "FGDProperty.generated.h"

USTRUCT()
struct FFGDPropertyChoice
{
	GENERATED_BODY()

	UPROPERTY(Category="FGDPropertyFlag", EditAnywhere)
	FString Value;

	UPROPERTY(Category="FGDPropertyFlag", EditAnywhere)
	FString Text;

	FORCEINLINE bool operator==(const FFGDPropertyChoice& Other) const
	{
		return Value == Other.Value && Text == Other.Text;
	}
};

USTRUCT()
struct FFGDPropertyFlag
{
	GENERATED_BODY()

	UPROPERTY(Category="FGDPropertyFlag", EditAnywhere)
	uint8 Value;

	UPROPERTY(Category="FGDPropertyFlag", EditAnywhere)
	FString Text;

	UPROPERTY(Category="FGDPropertyFlag", EditAnywhere)
	bool bIsTicked;

	FORCEINLINE bool operator==(const FFGDPropertyFlag& Other) const
	{
		return Value == Other.Value && Text == Other.Text && bIsTicked == Other.bIsTicked;
	}
};

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

	UPROPERTY(Category="FGDProperty", EditAnywhere)
	TArray<FFGDPropertyChoice> Choices;

	UPROPERTY(Category="FGDProperty", EditAnywhere)
	TArray<FFGDPropertyFlag> Flags;

	static UFGDProperty* CreateFromClass(const FString& InPath, const UClass* InClass);

	void SetOnObject(const FString& InValue, UObject* InObject) const;

	FString ToFGD() const;

protected:
	static UObject* GetContainerFromPath(const FString& InPath, UObject* InObject);
};
