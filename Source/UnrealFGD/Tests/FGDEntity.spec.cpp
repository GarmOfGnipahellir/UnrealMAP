#include "Engine/PointLight.h"
#include "Misc/AutomationTest.h"
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
}
