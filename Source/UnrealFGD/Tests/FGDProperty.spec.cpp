#include "Components/PointLightComponent.h"
#include "Engine/PointLight.h"
#include "Engine/StaticMeshActor.h"
#include "GameFramework/PlayerStart.h"
#include "Misc/AutomationTest.h"
#include "Tests/AutomationEditorCommon.h"
#include "UnrealFGD/Types/FGDProperty.h"

DEFINE_SPEC(
	FFGDProperty_Spec,
	"UnrealFGD.FGDProperty",
	EAutomationTestFlags::ProductFilter | EAutomationTestFlags::ApplicationContextMask
)

void FFGDProperty_Spec::Define()
{
	Describe(
		"CreateFromClass",
		[this]
		{
			It(
				"should create name property",
				[this]
				{
					const UFGDProperty* Property = UFGDProperty::CreateFromClass(
						"PlayerStartTag",
						APlayerStart::StaticClass()
					);
					TestEqual("Name", Property->Name, "player_start_tag");
					TestEqual("Path", Property->Path, "PlayerStartTag");
					TestEqual("Type", Property->Type, "string");
					TestEqual("Default", Property->Default, "None");
					TestEqual(
						"Description",
						Property->Description,
						"Used when searching for which playerstart to use."
					);
				}
			);

			It(
				"should create float property",
				[this]
				{
					const UFGDProperty* Property = UFGDProperty::CreateFromClass(
						"PointLightComponent:Intensity",
						APointLight::StaticClass()
					);
					TestEqual("Name", Property->Name, "intensity");
					TestEqual("Path", Property->Path, "PointLightComponent:Intensity");
					TestEqual("Type", Property->Type, "float");
					TestEqual("Default", Property->Default, "5000.000000");
					TestEqual(
						"Description",
						Property->Description,
						"Total energy that the light emits."
					);
				}
			);

			It(
				"should create bool property",
				[this]
				{
					const UFGDProperty* Property = UFGDProperty::CreateFromClass(
						"PointLightComponent:bUseInverseSquaredFalloff",
						APointLight::StaticClass()
					);
					TestEqual("Name", Property->Name, "use_inverse_squared_falloff");
					TestEqual("Path", Property->Path, "PointLightComponent:bUseInverseSquaredFalloff");
					TestEqual("Type", Property->Type, "choices");
					TestEqual("Default", Property->Default, "1");
					TestEqual(
						"Choices",
						Property->Choices,
						{
							{"0", "false"},
							{"1", "true"},
						}
					);
					TestEqual(
						"Description",
						Property->Description,
						"Whether to use physically based inverse squared distance falloff, where AttenuationRadius is only clamping the light's contribution. "
						"Disabling inverse squared falloff can be useful when placing fill lights (don't want a super bright spot near the light). "
						"When enabled, the light's Intensity is in units of lumens, where 1700 lumens is a 100W lightbulb. "
						"When disabled, the light's Intensity is a brightness scale."
					);
				}
			);

			// TODO: Handle flags
			xIt(
				"should create flags property",
				[this]
				{
					const UFGDProperty* Property = UFGDProperty::CreateFromClass(
						"StaticMeshComponent:ExcludeFromHLODLevels",
						AStaticMeshActor::StaticClass()
					);
					TestEqual("Name", Property->Name, "exclude_from_hlod_levels");
					TestEqual("Path", Property->Path, "StaticMeshComponent:ExcludeFromHLODLevels");
					TestEqual("Type", Property->Type, "flags");
					TestEqual("Default", Property->Default, "0");
					TestEqual(
						"Flags",
						Property->Flags,
						{
							{1 << 0, "HLOD0", false},
							{1 << 1, "HLOD0", false},
							{1 << 2, "HLOD0", false},
							{1 << 3, "HLOD0", false},
							{1 << 4, "HLOD0", false},
							{1 << 5, "HLOD0", false},
							{1 << 6, "HLOD0", false},
							{1 << 7, "HLOD0", false},
						}
					);
					TestEqual(
						"Description",
						Property->Description,
						"Which specific HLOD levels this component should be excluded from"
					);
				}
			);

			It(
				"should create color property",
				[this]
				{
					const UFGDProperty* Property = UFGDProperty::CreateFromClass(
						"PointLightComponent:LightColor",
						APointLight::StaticClass()
					);
					TestEqual("Name", Property->Name, "light_color");
					TestEqual("Path", Property->Path, "PointLightComponent:LightColor");
					TestEqual("Type", Property->Type, "color255");
					TestEqual("Default", Property->Default, "255 255 255");
					TestEqual(
						"Description",
						Property->Description,
						"Filter color of the light. Note that this can change the light's effective intensity."
					);
				}
			);

			It(
				"should create vector property",
				[this]
				{
					const UFGDProperty* Property = UFGDProperty::CreateFromClass(
						"StaticMeshComponent:RelativeLocation",
						AStaticMeshActor::StaticClass()
					);
					TestEqual("Name", Property->Name, "relative_location");
					TestEqual("Path", Property->Path, "StaticMeshComponent:RelativeLocation");
					TestEqual("Type", Property->Type, "vector");
					TestEqual("Default", Property->Default, "0.000000 0.000000 0.000000");
					TestEqual(
						"Description",
						Property->Description,
						"Location of the component relative to its parent"
					);
				}
			);
		}
	);

	Describe(
		"SetOnObject",
		[this]
		{
			It(
				"should set name property",
				[this]
				{
					UWorld* World = FAutomationEditorCommonUtils::CreateNewMap();

					const UFGDProperty* Property = UFGDProperty::CreateFromClass(
						"PlayerStartTag",
						APlayerStart::StaticClass()
					);

					APlayerStart* PlayerStart = World->SpawnActor<APlayerStart>();
					TestNotEqual("Pre set", PlayerStart->PlayerStartTag, FName("Cacodemon"));
					Property->SetOnObject("Cacodemon", PlayerStart);
					TestEqual("Post set", PlayerStart->PlayerStartTag, FName("Cacodemon"));
				}
			);

			It(
				"should set float property",
				[this]
				{
					UWorld* World = FAutomationEditorCommonUtils::CreateNewMap();

					const UFGDProperty* Property = UFGDProperty::CreateFromClass(
						"PointLightComponent:Intensity",
						APointLight::StaticClass()
					);

					APointLight* PointLight = World->SpawnActor<APointLight>();
					TestNotEqual("Pre set", PointLight->PointLightComponent->Intensity, 42.0f);
					Property->SetOnObject("42", PointLight);
					TestEqual("Post set", PointLight->PointLightComponent->Intensity, 42.0f);
				}
			);

			It(
				"should set bool property",
				[this]
				{
					UWorld* World = FAutomationEditorCommonUtils::CreateNewMap();

					const UFGDProperty* Property = UFGDProperty::CreateFromClass(
						"PointLightComponent:bUseInverseSquaredFalloff",
						APointLight::StaticClass()
					);

					APointLight* PointLight = World->SpawnActor<APointLight>();
					TestNotEqual("Pre set", (bool)PointLight->PointLightComponent->bUseInverseSquaredFalloff, false);
					Property->SetOnObject("0", PointLight);
					TestEqual("Post set", (bool)PointLight->PointLightComponent->bUseInverseSquaredFalloff, false);
				}
			);

			It(
				"should set color property",
				[this]
				{
					UWorld* World = FAutomationEditorCommonUtils::CreateNewMap();

					const UFGDProperty* Property = UFGDProperty::CreateFromClass(
						"PointLightComponent:LightColor",
						APointLight::StaticClass()
					);

					APointLight* PointLight = World->SpawnActor<APointLight>();
					TestNotEqual("Pre set", PointLight->PointLightComponent->LightColor, FColor(128, 128, 255));
					Property->SetOnObject("128 128 255", PointLight);
					TestEqual("Post set", PointLight->PointLightComponent->LightColor, FColor(128, 128, 255));
				}
			);

			It(
				"should set vector property",
				[this]
				{
					UWorld* World = FAutomationEditorCommonUtils::CreateNewMap();

					const UFGDProperty* Property = UFGDProperty::CreateFromClass(
						"PointLightComponent:RelativeLocation",
						APointLight::StaticClass()
					);

					APointLight* PointLight = World->SpawnActor<APointLight>();
					TestNotEqual("Pre set", PointLight->PointLightComponent->GetRelativeLocation(), FVector(1, 2, 3));
					Property->SetOnObject("1 2 3", PointLight);
					TestEqual("Post set", PointLight->PointLightComponent->GetRelativeLocation(), FVector(1, 2, 3));
				}
			);
		}
	);

	Describe(
		"ToFGD",
		[this]
		{
			It(
				"should return string property",
				[this]
				{
					const UFGDProperty* Property = UFGDProperty::CreateFromClass(
						"PlayerStartTag",
						APlayerStart::StaticClass()
					);
					TestEqual(
						"String property",
						Property->ToFGD(),
						"  player_start_tag(string) : \"PlayerStartTag\" : \"None\" : \"Used when searching for which playerstart to use.\""
					);
				}
			);

			It(
				"should return float property",
				[this]
				{
					const UFGDProperty* Property = UFGDProperty::CreateFromClass(
						"PointLightComponent:Intensity",
						APointLight::StaticClass()
					);
					TestEqual(
						"Float property",
						Property->ToFGD(),
						"  intensity(float) : \"PointLightComponent:Intensity\" : \"5000.000000\" : \"Total energy that the light emits.\""
					);
				}
			);

			It(
				"should return bool property",
				[this]
				{
					const UFGDProperty* Property = UFGDProperty::CreateFromClass(
						"PointLightComponent:bUseInverseSquaredFalloff",
						APointLight::StaticClass()
					);
					TestEqual(
						"Bool property",
						Property->ToFGD(),
						"  use_inverse_squared_falloff(choices) : \"PointLightComponent:bUseInverseSquaredFalloff\" : \"1\" : \""
						"Whether to use physically based inverse squared distance falloff, where AttenuationRadius is only clamping the light's contribution. "
						"Disabling inverse squared falloff can be useful when placing fill lights (don't want a super bright spot near the light). "
						"When enabled, the light's Intensity is in units of lumens, where 1700 lumens is a 100W lightbulb. "
						"When disabled, the light's Intensity is a brightness scale."
						"\" =\n"
						"  [\n"
						"    \"0\" : \"false\"\n"
						"    \"1\" : \"true\"\n"
						"  ]"
					);
				}
			);

			It(
				"should return color property",
				[this]
				{
					const UFGDProperty* Property = UFGDProperty::CreateFromClass(
						"PointLightComponent:LightColor",
						APointLight::StaticClass()
					);
					TestEqual(
						"Float property",
						Property->ToFGD(),
						"  light_color(color255) : \"PointLightComponent:LightColor\" : \"255 255 255\" : \"Filter color of the light. Note that this can change the light's effective intensity.\""
					);
				}
			);

			It(
				"should return vector property",
				[this]
				{
					const UFGDProperty* Property = UFGDProperty::CreateFromClass(
						"StaticMeshComponent:RelativeLocation",
						AStaticMeshActor::StaticClass()
					);
					TestEqual(
						"Float property",
						Property->ToFGD(),
						"  relative_location(vector) : \"StaticMeshComponent:RelativeLocation\" : \"0.000000 0.000000 0.000000\" : \"Location of the component relative to its parent\""
					);
				}
			);
		}
	);
}
