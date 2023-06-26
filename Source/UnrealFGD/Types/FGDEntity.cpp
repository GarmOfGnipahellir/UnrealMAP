// Fill out your copyright notice in the Description page of Project Settings.


#include "FGDEntity.h"

#include "FGDProperty.h"

UFGDEntity* UFGDEntity::CreateFromClass(const TArray<FString>& InPaths, const UClass* InClass)
{
	UFGDEntity* Result = NewObject<UFGDEntity>();

	Result->Name = InClass->GetName();

	for (const FString& Path : InPaths)
	{
		UFGDProperty* Property = UFGDProperty::CreateFromClass(Path, InClass);
		if (!Property) continue;
		Result->Properties.Add(Property->Name, Property);
	}

	return Result;
}

UFGDProperty* UFGDEntity::GetPropertyFromName(const FString& InName) const
{
	if (const auto ValuePtr = Properties.Find(InName))
	{
		return *ValuePtr;
	}
	return nullptr;
}

void UFGDEntity::SetPropertiesOnObject(const TMap<FString, FString>& InProperties, UObject* InObject) const
{
	for (const TTuple<FString, FString>& KVP : InProperties)
	{
		const UFGDProperty* Property = GetPropertyFromName(KVP.Key);
		if (!Property) continue;
		Property->SetOnObject(KVP.Value, InObject);
	}
}

FString UFGDEntity::ToFGD() const
{
	FString ClassPropertiesString = FString::Join(ClassProperties, TEXT(" "));
	FString Result = FString::Printf(TEXT("@PointClass %s = %s : \"\"\n[\n"), *ClassPropertiesString, *Name);
	for (const auto KVP : Properties)
	{
		Result += "  " + KVP.Value->ToFGD() + "\n";
	}
	Result += "]";
	return Result;
}
