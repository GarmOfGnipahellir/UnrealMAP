#include "UnrealMAP/Config/MAPConfigExporter.h"
#include "Misc/AutomationTest.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	ActorClassToEntityDefinition,
	"UnrealMAP.ActorClassToEntityDefinition",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter
)

bool ActorClassToEntityDefinition::RunTest(const FString& Parameters)
{
	TestEqual(
		"AActor",
		UMAPConfigExporter::ActorClassToEntityDefinition(AActor::StaticClass()),
		"@PointClass size(-16, -16, -16, 16, 16, 16) = Actor []"
	);

	return true;
}
