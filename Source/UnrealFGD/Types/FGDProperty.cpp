// Fill out your copyright notice in the Description page of Project Settings.


#include "FGDProperty.h"

#include "UnrealFGD/FGDUtils.h"

UFGDProperty* UFGDProperty::CreateFromClass(const FString& InPath, const UClass* InClass)
{
	TArray<FString> PathParts;
	InPath.ParseIntoArray(PathParts, TEXT(":"));

	UObject* Container = GetContainerFromPath(InPath, InClass->GetDefaultObject());
	const FProperty* Property = Container->GetClass()->FindPropertyByName(FName(PathParts[PathParts.Num() - 1]));

	UFGDProperty* Result = NewObject<UFGDProperty>();
	Result->Name = FGDUtils::PascalCaseToSnakeCase(Property->GetName());
	Result->Path = InPath;
	Result->Description = Property->GetToolTipText().ToString().Replace(TEXT("\n"), TEXT(" "));

	if (const FStrProperty* StrProperty = CastField<FStrProperty>(Property))
	{
		Result->Type = "string";
		Result->Default = StrProperty->GetPropertyValue_InContainer(Container);
	}
	else if (const FNameProperty* NameProperty = CastField<FNameProperty>(Property))
	{
		Result->Type = "string";
		Result->Default = NameProperty->GetPropertyValue_InContainer(Container).ToString();
	}
	else if (const FFloatProperty* FloatProperty = CastField<FFloatProperty>(Property))
	{
		Result->Type = "float";
		Result->Default = FloatProperty->GetNumericPropertyValueToString_InContainer(Container);
	}
	else if (const FBoolProperty* BoolProperty = CastField<FBoolProperty>(Property))
	{
		// TODO: Use choices
		Result->Type = "boolean";
		Result->Default = BoolProperty->GetPropertyValue(Container) ? "0" : "1";
	}
	else if (const FStructProperty* StructProperty = CastField<FStructProperty>(Property))
	{
		if (StructProperty->Struct->GetName() == "Color")
		{
			const FColor* Color = StructProperty->ContainerPtrToValuePtr<FColor>(Container);
			Result->Type = "color255";
			Result->Default = FString::Printf(TEXT("%d %d %d"), Color->R, Color->G, Color->B);
		}
		else if (StructProperty->Struct->GetName() == "Vector")
		{
			const FVector* Vector = StructProperty->ContainerPtrToValuePtr<FVector>(Container);
			Result->Type = "vector";
			Result->Default = FString::Printf(TEXT("%f %f %f"), Vector->X, Vector->Y, Vector->Z);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Unsupported struct property type '%s'."), *StructProperty->Struct->GetName())
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Unsupported property type '%s'."), *Property->GetClass()->GetName())
	}

	return Result;
}

void UFGDProperty::SetOnObject(const FString& InValue, UObject* InObject) const
{
	TArray<FString> PathParts;
	Path.ParseIntoArray(PathParts, TEXT(":"));

	UObject* Container = GetContainerFromPath(Path, InObject);

	const FProperty* Property = Container->GetClass()->FindPropertyByName(FName(PathParts[PathParts.Num() - 1]));
	if (const FStrProperty* StrProperty = CastField<FStrProperty>(Property))
	{
		StrProperty->SetValue_InContainer(Container, InValue);
		return;
	}
	if (const FNameProperty* NameProperty = CastField<FNameProperty>(Property))
	{
		NameProperty->SetValue_InContainer(Container, FName(InValue));
		return;
	}
	if (const FFloatProperty* FloatProperty = CastField<FFloatProperty>(Property))
	{
		FloatProperty->SetNumericPropertyValueFromString_InContainer(Container, *InValue);
		return;
	}
	if (const FBoolProperty* BoolProperty = CastField<FBoolProperty>(Property))
	{
		// TODO: Use choices
		const TOptional<bool> Boolean = FGDUtils::ParseBool(InValue);
		if (!Boolean)
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to parse bool from '%s'."), *InValue)
			return;
		}
		BoolProperty->SetPropertyValue_InContainer(Container, *Boolean);
		return;
	}
	if (const FStructProperty* StructProperty = CastField<FStructProperty>(Property))
	{
		if (StructProperty->Struct->GetName() == "Vector")
		{
			const TOptional<FVector> Vector = FGDUtils::ParseVector(InValue);
			if (!Vector)
			{
				UE_LOG(LogTemp, Warning, TEXT("Failed to parse vector from '%s'."), *InValue)
				return;
			}
			StructProperty->SetValue_InContainer(Container, new FVector(*Vector));
			return;
		}
		if (StructProperty->Struct->GetName() == "Color")
		{
			const TOptional<FColor> Color = FGDUtils::ParseColor(InValue);
			if (!Color)
			{
				UE_LOG(LogTemp, Warning, TEXT("Failed to parse color from '%s'."), *InValue)
				return;
			}
			StructProperty->SetValue_InContainer(Container, new FColor(*Color));
			return;
		}

		UE_LOG(LogTemp, Warning, TEXT("Unsupported struct property type '%s'."), *StructProperty->Struct->GetName())
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("Unsupported property type '%s'."), *Property->GetClass()->GetName())
}

FString UFGDProperty::ToFGD() const
{
	FString Result = FString::Printf(
		TEXT("%s(%s) : \"%s\" : \"%s\" : \"%s\""),
		*Name,
		*Type,
		*Path,
		*Default,
		*Description
	);
	// TODO: Handle choices/flags
	return Result;
}

UObject* UFGDProperty::GetContainerFromPath(const FString& InPath, UObject* InObject)
{
	TArray<FString> PathParts;
	InPath.ParseIntoArray(PathParts, TEXT(":"));

	UObject* Container = InObject;
	if (PathParts.Num() > 1)
	{
		for (int i = 0; i < PathParts.Num() - 1; ++i)
		{
			const FProperty* Property = Container->GetClass()->FindPropertyByName(FName(PathParts[i]));
			const FObjectProperty* ObjectProperty = CastField<FObjectProperty>(Property);
			if (!ObjectProperty)
			{
				UE_LOG(LogTemp, Error, TEXT("Can only visit UObject in paths '%s'."), *InPath)
				return nullptr;
			}
			Container = ObjectProperty->GetObjectPropertyValue_InContainer(Container);
		}
	}
	return Container;
}
