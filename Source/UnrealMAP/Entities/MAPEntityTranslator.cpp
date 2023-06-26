// Fill out your copyright notice in the Description page of Project Settings.


#include "MAPEntityTranslator.h"

#include "UnrealFGD/Types/FGDEntity.h"
#include "UnrealMAP/MAPLog.h"

UClass* UMAPEntityTranslator::GetSupportedClass_Implementation() const
{
	return AActor::StaticClass();
}

TArray<FString> UMAPEntityTranslator::GetPropertyPaths_Implementation(TSubclassOf<AActor> ActorClass) const
{
	return {};
}

TArray<FString> UMAPEntityTranslator::GetClassProperties_Implementation(TSubclassOf<AActor> ActorClass) const
{
	return {"size(-16 -16 -16, 16 16 16)"};
}

FString UMAPEntityTranslator::ToFGD(const TSubclassOf<AActor> ActorClass) const
{
	UFGDEntity* Entity = UFGDEntity::CreateFromClass(GetPropertyPaths(ActorClass), ActorClass);
	Entity->ClassProperties = GetClassProperties(ActorClass);
	return Entity->ToFGD();
}
