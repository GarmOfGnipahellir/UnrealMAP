#include "UnrealMAP/Entities/MAPEntityTranslator.h"

#include "Engine/PointLight.h"
#include "Misc/AutomationTest.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FMAPEntityTranslator_GetProperty,
	"UnrealMAP.MAPEntityTranslator.GetProperty",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter
)

bool FMAPEntityTranslator_GetProperty::RunTest(const FString& Parameters)
{
	TestTrue(
		"Actor Property",
		(bool)UMAPEntityTranslator::GetProperty("bEnabled", APointLight::StaticClass())
	);
	TestTrue(
		"Component Property",
		(bool)UMAPEntityTranslator::GetProperty("PointLightComponent-Intensity", APointLight::StaticClass())
	);

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FMAPEntityTranslator_PropertyToFGD,
	"UnrealMAP.MAPEntityTranslator.PropertyToFGD",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter
)

bool FMAPEntityTranslator_PropertyToFGD::RunTest(const FString& Parameters)
{
	const FPropertyRef IntensityProp = UMAPEntityTranslator::GetProperty(
		"PointLightComponent-Intensity",
		APointLight::StaticClass()
	);
	const FString IntensityFGD = UMAPEntityTranslator::PropertyToFGD(
		"PointLightComponent-Intensity",
		IntensityProp
	);
	TestEqual("Float Property", IntensityFGD, "Intensity(float) : \"PointLightComponent-Intensity\" : \"5000.000000\" : \"Total energy that the light emits.\"");
	
	const FPropertyRef EnabledProp = UMAPEntityTranslator::GetProperty(
		"bEnabled",
		APointLight::StaticClass()
	);
	const FString EnabledFGD = UMAPEntityTranslator::PropertyToFGD(
		"bEnabled",
		EnabledProp
	);
	TestEqual("Boolean Property", EnabledFGD, "bEnabled(boolean) : \"bEnabled\" : \"false\" : \"replicated copy of LightComponent's bEnabled property\"");

	return true;
}
