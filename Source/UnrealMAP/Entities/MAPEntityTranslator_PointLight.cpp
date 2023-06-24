// Fill out your copyright notice in the Description page of Project Settings.


#include "MAPEntityTranslator_PointLight.h"

#include "Components/PointLightComponent.h"
#include "Engine/PointLight.h"
#include "UnrealMAP/MAPLog.h"

UClass* UMAPEntityTranslator_PointLight::GetSupportedClass_Implementation() const
{
	return APointLight::StaticClass();
}

TArray<FMAPEntityProperty> UMAPEntityTranslator_PointLight::GetEntityProps_Implementation(
	const TSubclassOf<AActor> ActorClass) const
{
	const APointLight* PointLight = ActorClass->GetDefaultObject<APointLight>();
	if (!PointLight) return {};

	FProperty* Property = PointLight->PointLightComponent->StaticClass()->FindPropertyByName("Intensity");
	UE_LOG(LogMAP, Display, TEXT("APointLight::Intensity(%s)"), *Property->GetClass()->GetName());

	return {
		FMAPEntityProperty("intensity", EMAPEntityPropertyType::Float),
	};
}

TArray<FString> UMAPEntityTranslator_PointLight::GetPropNames_Implementation(TSubclassOf<AActor> ActorClass) const
{
	return {"PointLightComponent:Intensity"};
}
