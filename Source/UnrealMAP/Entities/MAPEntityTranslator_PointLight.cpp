// Fill out your copyright notice in the Description page of Project Settings.


#include "MAPEntityTranslator_PointLight.h"

#include "Components/PointLightComponent.h"
#include "Engine/PointLight.h"
#include "UnrealMAP/MAPLog.h"

UClass* UMAPEntityTranslator_PointLight::GetSupportedClass_Implementation() const
{
	return APointLight::StaticClass();
}

TArray<FString> UMAPEntityTranslator_PointLight::GetClassProperties_Implementation(TSubclassOf<AActor> ActorClass) const
{
	TArray<FString> Result = Super::GetClassProperties_Implementation(ActorClass);
	Result.Add("sphere(attenuation_radius)");
	return Result;
}

TArray<FString> UMAPEntityTranslator_PointLight::GetPropertyPaths_Implementation(TSubclassOf<AActor> ActorClass) const
{
	return {
		"PointLightComponent:Intensity",
		"PointLightComponent:LightColor",
		"PointLightComponent:AttenuationRadius",
		"PointLightComponent:bUseInverseSquaredFalloff",
	};
}
