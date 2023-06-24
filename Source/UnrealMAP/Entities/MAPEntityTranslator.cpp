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

FString UMAPEntityTranslator::ToFGD(const TSubclassOf<AActor> ActorClass) const
{
	TArray<FString> EntityProps;
	for (const FString& PropName : GetPropNames(ActorClass))
	{
		const FPropertyRef Property = GetProperty(PropName, ActorClass);
		if (!Property)
		{
			UE_LOG(
				LogMAP,
				Error,
				TEXT("MAPEntityTranslator: Couldn't find property '%s' for '%s'"),
				*PropName,
				*ActorClass->GetName()
			)
		}

		const FString PropertyFGD = PropertyToFGD(PropName, Property);
		if (PropertyFGD.IsEmpty())
		{
			UE_LOG(
				LogMAP,
				Error,
				TEXT("MAPEntityTranslator: Couldn't convert property '%s' for '%s' to FGD"),
				*PropName,
				*ActorClass->GetName()
			)
		}

		EntityProps.Add(PropertyFGD);
	}

	FStringFormatNamedArguments FormatArgs;
	FormatArgs.Add(TEXT("ClassType"), ClassTypeToString(GetClassType(ActorClass)));
	FormatArgs.Add(TEXT("ClassProps"), ClassPropsToString(GetClassProps(ActorClass)));
	FormatArgs.Add(TEXT("EntityName"), GetEntityName(ActorClass));
	FormatArgs.Add(TEXT("EntityProps"), FString::Join(EntityProps, TEXT("\n\n")));
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

FPropertyRef UMAPEntityTranslator::GetProperty(const FString& PropertyName, const TSubclassOf<AActor> ActorClass)
{
	const AActor* Actor = ActorClass->GetDefaultObject<AActor>();
	check(Actor)

	TArray<FString> NameParts;
	PropertyName.ParseIntoArray(NameParts, TEXT("-"));
	if (NameParts.Num() == 1)
	{
		return FPropertyRef(Actor, ActorClass->FindPropertyByName(FName(PropertyName)));
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
			return FPropertyRef();
		}

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
			return FPropertyRef();
		}

		return FPropertyRef(Component, Component->GetClass()->FindPropertyByName(FName(NameParts[1])));
	}

	UE_LOG(
		LogMAP,
		Error,
		TEXT("MAPEntityTranslator: Failed to parse property name '%s'"),
		*PropertyName
	);
	return FPropertyRef();
}

FString UMAPEntityTranslator::PropertyToFGD(
	const FString& PropertyName,
	const FPropertyRef& PropertyRef)
{
	if (!PropertyRef)
	{
		UE_LOG(
			LogMAP,
			Error,
			TEXT("MAPEntityTranslator: Invalid property reference."),
		);
		return "";
	}

	FString PropertyType = "";
	FString PropertyDefault = "";
	if (const FFloatProperty* FloatProperty = CastField<FFloatProperty>(PropertyRef.Property))
	{
		PropertyType = "float";
		PropertyDefault = FloatProperty->GetNumericPropertyValueToString_InContainer(PropertyRef.Container);
	}
	else if (const FBoolProperty* BoolProperty = CastField<FBoolProperty>(PropertyRef.Property))
	{
		PropertyType = "boolean";
		PropertyDefault = BoolProperty->GetPropertyValue_InContainer(PropertyRef.Container) ? "true" : "false";
	}

	if (PropertyType.IsEmpty())
	{
		UE_LOG(
			LogMAP,
			Error,
			TEXT("MAPEntityTranslator: Failed to get property type from '%s' for '%s'."),
			*PropertyName,
			*PropertyRef.Container->GetClass()->GetName()
		);
		return "";
	}

	FStringFormatOrderedArguments FormatArgs;
	FormatArgs.Add(PropertyRef.Property->GetName());
	FormatArgs.Add(PropertyType);
	FormatArgs.Add(PropertyName);
	FormatArgs.Add(PropertyDefault);
	FormatArgs.Add(PropertyRef.Property->GetToolTipText(true).ToString());
	return FString::Format(TEXT("{0}({1}) : \"{2}\" : \"{3}\" : \"{4}\""), FormatArgs);
}
