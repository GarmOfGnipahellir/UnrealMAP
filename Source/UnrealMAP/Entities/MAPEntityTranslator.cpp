// Fill out your copyright notice in the Description page of Project Settings.


#include "MAPEntityTranslator.h"

#include "UnrealMAP/MAPLog.h"

UClass* UMAPEntityTranslator::GetSupportedClass_Implementation() const
{
	return AActor::StaticClass();
}

EMAPClassType UMAPEntityTranslator::GetClassType_Implementation(const TSubclassOf<AActor> ActorClass) const
{
	return EMAPClassType::PointClass;
}

TArray<FMAPClassProperty> UMAPEntityTranslator::GetClassProps_Implementation(const TSubclassOf<AActor> ActorClass) const
{
	return {FMAPClassProperty("size", "-16 -16 -16, 16 16 16")};
}

FString UMAPEntityTranslator::GetEntityName_Implementation(const TSubclassOf<AActor> ActorClass) const
{
	return ActorClass->GetName();
}

TArray<FMAPEntityProperty> UMAPEntityTranslator::GetEntityProps_Implementation(
	const TSubclassOf<AActor> ActorClass) const
{
	return {};
}

TArray<FString> UMAPEntityTranslator::GetPropNames_Implementation(TSubclassOf<AActor> ActorClass) const
{
	return {};
}

FString UMAPEntityTranslator::ToString(const TSubclassOf<AActor> ActorClass) const
{
	FStringFormatNamedArguments FormatArgs;
	FormatArgs.Add(TEXT("ClassType"), ClassTypeToString(GetClassType(ActorClass)));
	FormatArgs.Add(TEXT("ClassProps"), ClassPropsToString(GetClassProps(ActorClass)));
	FormatArgs.Add(TEXT("EntityName"), GetEntityName(ActorClass));
	FormatArgs.Add(TEXT("EntityProps"), EntityPropsToString(GetEntityProps(ActorClass)));
	return FString::Format(TEXT("@{ClassType} {ClassProps} = {EntityName} [\n{EntityProps}\n]"), FormatArgs);
}

FString UMAPEntityTranslator::ClassTypeToString(const EMAPClassType& ClassType)
{
	switch (ClassType)
	{
	case EMAPClassType::PointClass:
		return "PointClass";
	}
	checkNoEntry()
	return "";
}

FString UMAPEntityTranslator::ClassPropToString(const FMAPClassProperty& ClassProp)
{
	return FString::Printf(TEXT("%s(%s)"), *ClassProp.Name, *ClassProp.Value);
}

FString UMAPEntityTranslator::ClassPropsToString(const TArray<FMAPClassProperty>& ClassProps)
{
	TArray<FString> PropStrings;
	PropStrings.Reserve(ClassProps.Num());
	for (const FMAPClassProperty& Prop : ClassProps)
	{
		PropStrings.Add(ClassPropToString(Prop));
	}
	return FString::Join(PropStrings, TEXT(" "));
}

FString UMAPEntityTranslator::EntityPropTypeToString(const EMAPEntityPropertyType& EntityPropType)
{
	switch (EntityPropType)
	{
	case EMAPEntityPropertyType::String:
		return "string";
	case EMAPEntityPropertyType::Integer:
		return "integer";
	case EMAPEntityPropertyType::Float:
		return "float";
	case EMAPEntityPropertyType::Boolean:
		return "boolean";
	}
	checkNoEntry()
	return "";
}

FString UMAPEntityTranslator::EntityPropToString(const FMAPEntityProperty& EntityProp)
{
	FStringFormatOrderedArguments FormatArgs;
	FormatArgs.Add(EntityProp.Name);
	FormatArgs.Add(EntityPropTypeToString(EntityProp.Type));
	FormatArgs.Add(EntityProp.DisplayName);
	FormatArgs.Add(EntityProp.DefaultValue);
	FormatArgs.Add(EntityProp.Description);
	return FString::Format(TEXT("{0}({1}) : \"{2}\" : \"{3}\" : \"{4}\""), FormatArgs);
}

FString UMAPEntityTranslator::EntityPropsToString(const TArray<FMAPEntityProperty>& EntityProps)
{
	TArray<FString> PropStrings;
	PropStrings.Reserve(EntityProps.Num());
	for (const FMAPEntityProperty& Prop : EntityProps)
	{
		PropStrings.Add(EntityPropToString(Prop));
	}
	return FString::Join(PropStrings, TEXT("\n"));
}

FProperty* UMAPEntityTranslator::GetProperty(const FString& PropertyName, TSubclassOf<AActor> ActorClass)
{
	TArray<FString> NameParts;
	PropertyName.ParseIntoArray(NameParts, TEXT(":"));
	if (NameParts.Num() == 1)
	{
		return ActorClass->FindPropertyByName(FName(PropertyName));
	}
	if (NameParts.Num() == 2)
	{
		const FObjectProperty* ComponentProperty = CastField<FObjectProperty>(
			ActorClass->FindPropertyByName(FName(NameParts[0]))
		);
		if (!ComponentProperty)
		{
			UE_LOG(
				LogMAP,
				Error,
				TEXT("MAPEntityTranslator: Couldn't find object property '%s' for '%s'"),
				*NameParts[0],
				*ActorClass->GetName()
			);
			return nullptr;
		}

		const AActor* Actor = ActorClass->GetDefaultObject<AActor>();
		check(Actor)

		const UObject* Component = ComponentProperty->GetObjectPropertyValue_InContainer(Actor);
		if (!Component)
		{
			UE_LOG(
				LogMAP,
				Error,
				TEXT("MAPEntityTranslator: Couldn't find component '%s' for '%s'"),
				*NameParts[0],
				*ActorClass->GetName()
			);
			return nullptr;
		}

		return Component->GetClass()->FindPropertyByName(FName(NameParts[1]));
	}

	UE_LOG(
		LogMAP,
		Error,
		TEXT("MAPEntityTranslator: Failed to parse property name '%s'"),
		*PropertyName
	);
	return nullptr;
}

FString UMAPEntityTranslator::PropertyToFGD(
	const FString& PropertyName,
	FProperty* Property,
	TSubclassOf<AActor> ActorClass)
{
	FStringFormatOrderedArguments FormatArgs;
	FormatArgs.Add(PropertyName);
	FormatArgs.Add(Property->GetClass()->GetName());
	FormatArgs.Add(Property->GetName());
	FormatArgs.Add(""); // TODO: Get default value
	FormatArgs.Add(""); // TODO: Get tooltip as description
	return FString::Format(TEXT("{0}({1}) : \"{2}\" : \"{3}\" : \"{4}\""), FormatArgs);
}
