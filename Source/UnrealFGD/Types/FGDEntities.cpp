// Fill out your copyright notice in the Description page of Project Settings.


#include "FGDEntities.h"

#include "FGDEntity.h"

UFGDEntity* UFGDEntities::GetEntityFromObjectClass(const UClass* InClass) const
{
	const FString ClassName = InClass->GetName();
	if (const auto ValuePtr = Entities.Find(ClassName))
	{
		return *ValuePtr;
	}
	return nullptr;
}
