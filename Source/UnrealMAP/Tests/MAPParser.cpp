#include "UnrealMAP/MAPParser.h"
#include "Misc/AutomationTest.h"
#include "UnrealMAP/MAPModel.h"

class FAutomationTestParser : public FAutomationTestBase
{
public:
	FAutomationTestParser(const FString& InName, bool bInComplexTask)
		: FAutomationTestBase(InName, bInComplexTask)
	{
	}

	template <typename InType, typename OutType>
	bool TestEqualParsed(TTuple<InType, OutType> Actual, InType ExpectedIn, OutType ExpectedOut)
	{
		bool Result = true;
		auto [In, Out] = Actual;
		if (!TestEqual("In", In, ExpectedIn))
		{
			Result = false;
		}
		if (!TestEqual("Out", Out, ExpectedOut))
		{
			Result = false;
		}
		return Result;
	}
};

IMPLEMENT_CUSTOM_SIMPLE_AUTOMATION_TEST(
	MAPParser_Tag,
	FAutomationTestParser,
	"UnrealMAP.MAPParser.Tag",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter
)

bool MAPParser_Tag::RunTest(const FString& Parameters)
{
	TestEqualParsed<FString, FString>(FMAPParser::Tag("Hello", "Hello world!"), " world!", "Hello");

	try
	{
		FMAPParser::Tag("Bye", "Hello world!");
		AddError("Expected to fail");
	}
	catch (const std::exception&)
	{
	}

	return true;
}

IMPLEMENT_CUSTOM_SIMPLE_AUTOMATION_TEST(
	MAPParser_NotChar,
	FAutomationTestParser,
	"UnrealMAP.MAPParser.NotChar",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter
)

bool MAPParser_NotChar::RunTest(const FString& Parameters)
{
	TestEqualParsed<FString, FString>(FMAPParser::NotChar('w', "Hello world!"), "world!", "Hello ");

	try
	{
		FMAPParser::NotChar('H', "Hello world!", true);
		AddError("Expected to fail");
	}
	catch (const std::exception&)
	{
	}

	return true;
}

IMPLEMENT_CUSTOM_SIMPLE_AUTOMATION_TEST(
	MAPParser_Float,
	FAutomationTestParser,
	"UnrealMAP.MAPParser.Float",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter
)

bool MAPParser_Float::RunTest(const FString& Parameters)
{
	TestEqualParsed<FString, float>(FMAPParser::Float("-64"), "", -64.0f);
	TestEqualParsed<FString, float>(FMAPParser::Float("-64 0 256"), " 0 256", -64.0f);
	TestEqualParsed<FString, float>(FMAPParser::Float("256"), "", 256.0f);
	TestEqualParsed<FString, float>(FMAPParser::Float("-59.712812921100124"), "", -59.712812921100124f);
	TestEqualParsed<FString, float>(FMAPParser::Float("272.56441888656036"), "", 272.56441888656036f);
	TestEqualParsed<FString, float>(FMAPParser::Float("-0.0503174"), "", -0.0503174f);
	TestEqualParsed<FString, float>(FMAPParser::Float("0.382199 -0.922709"), " -0.922709", 0.382199f);

	try
	{
		FMAPParser::Float("abc");
		AddError("Expected to fail");
	}
	catch (const std::exception&)
	{
	}

	try
	{
		FMAPParser::Float("");
		AddError("Expected to fail");
	}
	catch (const std::exception&)
	{
	}

	return true;
}

IMPLEMENT_CUSTOM_SIMPLE_AUTOMATION_TEST(
	MAPParser_Whitespace,
	FAutomationTestParser,
	"UnrealMAP.MAPParser.Whitespace",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter
)

bool MAPParser_Whitespace::RunTest(const FString& Parameters)
{
	TestEqualParsed<FString, FString>(FMAPParser::Whitespace(" abc"), "abc", " ");
	TestEqualParsed<FString, FString>(FMAPParser::Whitespace("\tabc"), "abc", "\t");
	TestEqualParsed<FString, FString>(FMAPParser::Whitespace("\nabc"), "abc", "\n");
	TestEqualParsed<FString, FString>(FMAPParser::Whitespace(" \t\nabc"), "abc", " \t\n");

	try
	{
		FMAPParser::Whitespace("", true);
	}
	catch (const std::exception&)
	{
	}

	try
	{
		FMAPParser::Whitespace("abc  ", true);
	}
	catch (const std::exception&)
	{
	}

	return true;
}

IMPLEMENT_CUSTOM_SIMPLE_AUTOMATION_TEST(
	MAPParser_NotWhitespace,
	FAutomationTestParser,
	"UnrealMAP.MAPParser.NotWhitespace",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter
)

bool MAPParser_NotWhitespace::RunTest(const FString& Parameters)
{
	TestEqualParsed<FString, FString>(FMAPParser::NotWhitespace("a/-_b.c "), " ", "a/-_b.c");
	TestEqualParsed<FString, FString>(FMAPParser::NotWhitespace("abc\t"), "\t", "abc");
	TestEqualParsed<FString, FString>(FMAPParser::NotWhitespace("abc\n"), "\n", "abc");
	TestEqualParsed<FString, FString>(FMAPParser::NotWhitespace("abc \t\n"), " \t\n", "abc");

	try
	{
		FMAPParser::NotWhitespace("", true);
	}
	catch (const std::exception&)
	{
	}

	try
	{
		FMAPParser::NotWhitespace("  abc", true);
	}
	catch (const std::exception&)
	{
	}

	return true;
}

IMPLEMENT_CUSTOM_SIMPLE_AUTOMATION_TEST(
	MAPParser_Vector,
	FAutomationTestParser,
	"UnrealMAP.MAPParser.Vector",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter
)

bool MAPParser_Vector::RunTest(const FString& Parameters)
{
	TestEqualParsed<FString, FVector>(FMAPParser::Vector("-64 0 256"), "", FVector(-64, 0, 256));
	TestEqualParsed<FString, FVector>(
		FMAPParser::Vector("-0.0503174 -0.382199 -0.922709"),
		"",
		FVector(-0.0503174f, -0.382199f, -0.922709f)
	);

	return true;
}

IMPLEMENT_CUSTOM_SIMPLE_AUTOMATION_TEST(
	MAPParser_MAPPlane,
	FAutomationTestParser,
	"UnrealMAP.MAPParser.MAPPlane",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter
)

bool MAPParser_MAPPlane::RunTest(const FString& Parameters)
{
	TestEqualParsed<FString, FMAPPlane>(
		FMAPParser::MAPPlane("( -240 -64 -16 ) ( -240 -63 -16 ) ( -240 -64 -15 )"),
		"",
		FMAPPlane{FVector(-240, -64, -16), FVector(-240, -63, -16), FVector(-240, -64, -15)}
	);
	TestEqualParsed<FString, FMAPPlane>(
		FMAPParser::MAPPlane("(-240 -64 -16) (-240 -63 -16) (-240 -64 -15)"),
		"",
		FMAPPlane{FVector(-240, -64, -16), FVector(-240, -63, -16), FVector(-240, -64, -15)}
	);
	TestEqualParsed<FString, FMAPPlane>(
		FMAPParser::MAPPlane("(0 0 0) (0 1 0) (0 0 1) __TB_empty [ 1 0 -0 0 ] [ 0 -0 -1 0 ] 0 1 1"),
		" __TB_empty [ 1 0 -0 0 ] [ 0 -0 -1 0 ] 0 1 1",
		FMAPPlane{FVector(0, 0, 0), FVector(0, 1, 0), FVector(0, 0, 1)}
	);

	return true;
}

IMPLEMENT_CUSTOM_SIMPLE_AUTOMATION_TEST(
	MAPParser_MAPFace,
	FAutomationTestParser,
	"UnrealMAP.MAPParser.MAPFace",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter
)

bool MAPParser_MAPFace::RunTest(const FString& Parameters)
{
	TestEqualParsed<FString, FMAPFace>(
		FMAPParser::MAPFace(
			"( -240 -64 -16 ) ( -240 -63 -16 ) ( -240 -64 -15 ) __TB_empty [ 0 -1 0 4 ] [ -0 -0 -1 8 ] 45 1 2"
		),
		"",
		FMAPFace{
			FMAPPlane{FVector(-240, -64, -16), FVector(-240, -63, -16), FVector(-240, -64, -15)},
			"__TB_empty",
			FVector(0, -1, 0),
			FVector(0, 0, -1),
			FVector2d(4, 8),
			45,
			FVector2d(1, 2)
		}
	);

	return true;
}

IMPLEMENT_CUSTOM_SIMPLE_AUTOMATION_TEST(
	MAPParser_MAPBrush,
	FAutomationTestParser,
	"UnrealMAP.MAPParser.MAPBrush",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter
)

bool MAPParser_MAPBrush::RunTest(const FString& Parameters)
{
	TestEqualParsed<FString, FMAPBrush>(
		FMAPParser::MAPBrush(
			"{\n"
			"( -240 -64 -16 ) ( -240 -63 -16 ) ( -240 -64 -15 ) __TB_empty [ 0 -1 0 0 ] [ -0 -0 -1 0 ] 0 1 1\n"
			"( -64 -192 -16 ) ( -64 -192 -15 ) ( -63 -192 -16 ) __TB_empty [ 1 0 -0 0 ] [ 0 -0 -1 0 ] 0 1 1\n"
			"( -64 -64 -16 ) ( -63 -64 -16 ) ( -64 -63 -16 ) __TB_empty [ -1 0 0 0 ] [ -0 -1 -0 0 ] 0 1 1\n"
			"( 64 64 0 ) ( 64 65 0 ) ( 65 64 0 ) __TB_empty [ 1 0 0 0 ] [ 0 -1 0 0 ] 0 1 1\n"
			"( 64 192 16 ) ( 65 192 16 ) ( 64 192 17 ) __TB_empty [ -1 0 0 0 ] [ 0 0 -1 0 ] 0 1 1\n"
			"( 256 64 16 ) ( 256 64 17 ) ( 256 65 16 ) __TB_empty [ 0 1 0 0 ] [ 0 0 -1 0 ] 0 1 1\n"
			"}"
		),
		"",
		FMAPBrush{
			{
				FMAPParser::MAPFace(
					"( -240 -64 -16 ) ( -240 -63 -16 ) ( -240 -64 -15 ) __TB_empty [ 0 -1 0 0 ] [ -0 -0 -1 0 ] 0 1 1"
				).Value,
				FMAPParser::MAPFace(
					"( -64 -192 -16 ) ( -64 -192 -15 ) ( -63 -192 -16 ) __TB_empty [ 1 0 -0 0 ] [ 0 -0 -1 0 ] 0 1 1"
				).Value,
				FMAPParser::MAPFace(
					"( -64 -64 -16 ) ( -63 -64 -16 ) ( -64 -63 -16 ) __TB_empty [ -1 0 0 0 ] [ -0 -1 -0 0 ] 0 1 1"
				).Value,
				FMAPParser::MAPFace(
					"( 64 64 0 ) ( 64 65 0 ) ( 65 64 0 ) __TB_empty [ 1 0 0 0 ] [ 0 -1 0 0 ] 0 1 1"
				).Value,
				FMAPParser::MAPFace(
					"( 64 192 16 ) ( 65 192 16 ) ( 64 192 17 ) __TB_empty [ -1 0 0 0 ] [ 0 0 -1 0 ] 0 1 1"
				).Value,
				FMAPParser::MAPFace(
					"( 256 64 16 ) ( 256 64 17 ) ( 256 65 16 ) __TB_empty [ 0 1 0 0 ] [ 0 0 -1 0 ] 0 1 1"
				).Value
			}
		}
	);

	return true;
}

IMPLEMENT_CUSTOM_SIMPLE_AUTOMATION_TEST(
	MAPParser_MAPProperty,
	FAutomationTestParser,
	"UnrealMAP.MAPParser.MAPProperty",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter
)

bool MAPParser_MAPProperty::RunTest(const FString& Parameters)
{
	TestEqualParsed<FString, TTuple<FString, FString>>(
		FMAPParser::MAPProperty("\"classname\" \"worldspawn\""),
		"",
		TTuple<FString, FString>("classname", "worldspawn")
	);
	TestEqualParsed<FString, TTuple<FString, FString>>(
		FMAPParser::MAPProperty("\"_tb_textures\" \"textures/base\""),
		"",
		TTuple<FString, FString>("_tb_textures", "textures/base")
	);

	return true;
}

IMPLEMENT_CUSTOM_SIMPLE_AUTOMATION_TEST(
	MAPParser_MAPProperties,
	FAutomationTestParser,
	"UnrealMAP.MAPParser.MAPProperties",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter
)

bool MAPParser_MAPProperties::RunTest(const FString& Parameters)
{
	auto [In, Out] = FMAPParser::MAPProperties(
		"\"classname\" \"worldspawn\"\n"
		"\"mapversion\" \"220\"\n"
		"\"_tb_textures\" \"textures/base\""
	);

	TestTrue(
		"Maps equal",
		Out.OrderIndependentCompareEqual(
			TMap<FString, FString>(
				{
					TTuple<FString, FString>("classname", "worldspawn"),
					TTuple<FString, FString>("mapversion", "220"),
					TTuple<FString, FString>("_tb_textures", "textures/base")
				}
			)
		)
	);

	return true;
}
