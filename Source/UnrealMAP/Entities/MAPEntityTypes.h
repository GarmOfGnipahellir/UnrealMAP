// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MAPEntityTypes.generated.h"

UENUM(BlueprintType)
enum class EMAPClassType : uint8
{
	PointClass,
};

USTRUCT(BlueprintType)
struct FMAPClassProperty
{
	GENERATED_BODY()

	FMAPClassProperty() : Name(""), Value("") { }

	FMAPClassProperty(const FString& InName, const FString& InValue) : Name(InName), Value(InValue) { }

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Value;
};

UENUM(BlueprintType)
enum class EMAPEntityPropertyType : uint8
{
	String,
	Integer,
	Float,
	Boolean,
};

USTRUCT(BlueprintType)
struct FMAPEntityProperty
{
	GENERATED_BODY()

	FMAPEntityProperty() :
		Name(""),
		Type(EMAPEntityPropertyType::String),
		DisplayName(""),
		DefaultValue(""),
		Description("") { }

	FMAPEntityProperty(
		const FString& InName,
		const EMAPEntityPropertyType& InType) :
		Name(InName),
		Type(InType),
		DisplayName(""),
		DefaultValue(""),
		Description("") { }

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EMAPEntityPropertyType Type;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString DisplayName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString DefaultValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Description;
};

struct FPropertyRef
{
	FPropertyRef(): Container(nullptr), Property(nullptr) { }

	FPropertyRef(
		const UObject* InContainer,
		const FProperty* InProperty): Container(InContainer), Property(InProperty) { }

	const UObject* Container;
	const FProperty* Property;

	explicit operator bool() const { return Container && Property; }
};
