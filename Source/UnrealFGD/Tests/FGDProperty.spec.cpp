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
						"Filter color of the light.\nNote that this can change the light's effective intensity."
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
}
