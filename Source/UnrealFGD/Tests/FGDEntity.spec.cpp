﻿#include "Components/PointLightComponent.h"
#include "Engine/PointLight.h"
#include "Misc/AutomationTest.h"
#include "Tests/AutomationEditorCommon.h"
#include "UnrealFGD/Types/FGDEntity.h"

DEFINE_SPEC(
	FFGDEntity_Spec,
	"UnrealFGD.FGDEntity",
	EAutomationTestFlags::ProductFilter | EAutomationTestFlags::ApplicationContextMask
)

void FFGDEntity_Spec::Define()
{
	Describe(
		"CreateFromClass",
		[this]
		{
			It(
				"should populate properties",
				[this]
				{
					const UFGDEntity* Entity = UFGDEntity::CreateFromClass(
						{
							"PointLightComponent:Intensity",
							"PointLightComponent:LightColor",
							"PointLightComponent:AttenuationRadius",
						},
						APointLight::StaticClass()
					);
					TestEqual("Num properties", Entity->Properties.Num(), 3);
				}
			);
		}
	);

	Describe(
		"SetPropertiesOnObject",
		[this]
		{
			It(
				"should populate properties",
				[this]
				{
					UWorld* World = FAutomationEditorCommonUtils::CreateNewMap();

					const UFGDEntity* Entity = UFGDEntity::CreateFromClass(
						{
							"PointLightComponent:Intensity",
							"PointLightComponent:LightColor",
							"PointLightComponent:AttenuationRadius",
						},
						APointLight::StaticClass()
					);

					APointLight* PointLight = World->SpawnActor<APointLight>();

					TestNotEqual("Intensity pre set", PointLight->PointLightComponent->Intensity, 42.0f);
					TestNotEqual(
						"LightColor pre set",
						PointLight->PointLightComponent->LightColor,
						FColor(128, 128, 255)
					);
					TestNotEqual(
						"AttenuationRadius pre set",
						PointLight->PointLightComponent->AttenuationRadius,
						42.0f
					);

					Entity->SetPropertiesOnObject(
						{
							{"intensity", "42"},
							{"light_color", "128 128 255"},
							{"attenuation_radius", "42"},
						},
						PointLight
					);

					TestEqual("Intensity post set", PointLight->PointLightComponent->Intensity, 42.0f);
					TestEqual(
						"LightColor post set",
						PointLight->PointLightComponent->LightColor,
						FColor(128, 128, 255)
					);
					TestEqual("AttenuationRadius post set", PointLight->PointLightComponent->AttenuationRadius, 42.0f);
				}
			);
		}
	);

	Describe(
		"ToFGD",
		[this]
		{
			It(
				"should return vector property",
				[this]
				{
					UFGDEntity* Entity = UFGDEntity::CreateFromClass(
						{
							"PointLightComponent:Intensity",
							"PointLightComponent:LightColor",
							"PointLightComponent:AttenuationRadius",
						},
						APointLight::StaticClass()
					);
					Entity->ClassProperties = {"size(16 16 16, -16 -16 -16)", "sphere(attenuation_radius)"};
					TestEqual(
						"Float property",
						Entity->ToFGD(),
						"@PointClass size(16 16 16, -16 -16 -16) sphere(attenuation_radius) = PointLight : \"\"\n"
						"[\n"
						"  intensity(float) : \"PointLightComponent:Intensity\" : \"5000.000000\" : \"Total energy that the light emits.\"\n"
						"  light_color(color255) : \"PointLightComponent:LightColor\" : \"255 255 255\" : \"Filter color of the light. Note that this can change the light's effective intensity.\"\n"
						"  attenuation_radius(float) : \"PointLightComponent:AttenuationRadius\" : \"1000.000000\" : \"Bounds the light's visible influence. This clamping of the light's influence is not physically correct but very important for performance, larger lights cost more.\"\n"
						"]"
					);
				}
			);
		}
	);
}
