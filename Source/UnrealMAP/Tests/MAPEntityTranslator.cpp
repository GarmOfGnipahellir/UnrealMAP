#include "UnrealMAP/Entities/MAPEntityTranslator.h"

#include "Engine/PointLight.h"
#include "Misc/AutomationTest.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	MAPEntityTranslator_GetProperty,
	"UnrealMAP.MAPEntityTranslator.GetProperty",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter
)

bool MAPEntityTranslator_GetProperty::RunTest(const FString& Parameters)
{
	TestNotNull(
		"Actor Property",
		UMAPEntityTranslator::GetProperty("bEnabled", APointLight::StaticClass())
	);
	TestNotNull(
		"Component Property",
		UMAPEntityTranslator::GetProperty("PointLightComponent:Intensity", APointLight::StaticClass())
	);

	return true;
}
