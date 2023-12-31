﻿#include "UnrealMAP/MAPParser.h"
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
	TestEqualParsed<FString, float>(FMAPParser::Float("2.22045e-16"), "", 0.000000000000000222045f);

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
	MAPParser_Comment,
	FAutomationTestParser,
	"UnrealMAP.MAPParser.Comment",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter
)

bool MAPParser_Comment::RunTest(const FString& Parameters)
{
	TestEqualParsed<FString, FString>(FMAPParser::Comment("// Game: Unreal"), "", " Game: Unreal");
	TestEqualParsed<FString, FString>(FMAPParser::Comment("// entity 0"), "", " entity 0");
	TestEqualParsed<FString, FString>(FMAPParser::Comment("// brush 0"), "", " brush 0");
	TestEqualParsed<FString, FString>(FMAPParser::Comment("// foo: bar\nbaz"), "\nbaz", " foo: bar");

	return true;
}

IMPLEMENT_CUSTOM_SIMPLE_AUTOMATION_TEST(
	MAPParser_Comments,
	FAutomationTestParser,
	"UnrealMAP.MAPParser.Comments",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter
)

bool MAPParser_Comments::RunTest(const FString& Parameters)
{
	TestEqualParsed<FString, TArray<FString>>(
		FMAPParser::Comments(
			"// Game: Unreal\n"
			"// entity 0\n"
			"// brush 0"
		),
		"",
		{" Game: Unreal", " entity 0", " brush 0"}
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
	TestEqualParsed<FString, FMAPFace>(
		FMAPParser::MAPFace(
			"( -240 -64 -16 ) ( -240 -63 -16 ) ( -240 -64 -15 ) base/grid [ 0 -1 0 0 ] [ -0 -0 -1 0 ] 0 1 1"
		),
		"",
		FMAPFace{
			FMAPPlane{FVector(-240, -64, -16), FVector(-240, -63, -16), FVector(-240, -64, -15)},
			"base/grid",
			FVector(0, -1, 0),
			FVector(0, 0, -1),
			FVector2d(0, 0),
			0,
			FVector2d(1, 1)
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
	TestEqualParsed<FString, TTuple<FString, FString>>(
		FMAPParser::MAPProperty("\"_tb_textures\" \"textures/base;textures/special\""),
		"",
		TTuple<FString, FString>("_tb_textures", "textures/base;textures/special")
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

IMPLEMENT_CUSTOM_SIMPLE_AUTOMATION_TEST(
	MAPParser_MAPEntity,
	FAutomationTestParser,
	"UnrealMAP.MAPParser.MAPEntity",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter
)

bool MAPParser_MAPEntity::RunTest(const FString& Parameters)
{
	TestEqualParsed<FString, FMAPEntity>(
		FMAPParser::MAPEntity(
			"// entity 0\n"
			"{\n"
			"\"classname\" \"worldspawn\"\n"
			"\"mapversion\" \"220\"\n"
			"\"_tb_textures\" \"textures/base\"\n"
			"// brush 0\n"
			"{\n"
			"( -240 -64 -16 ) ( -240 -63 -16 ) ( -240 -64 -15 ) __TB_empty [ 0 -1 0 0 ] [ -0 -0 -1 0 ] 0 1 1\n"
			"( -64 -192 -16 ) ( -64 -192 -15 ) ( -63 -192 -16 ) __TB_empty [ 1 0 -0 0 ] [ 0 -0 -1 0 ] 0 1 1\n"
			"( -64 -64 -16 ) ( -63 -64 -16 ) ( -64 -63 -16 ) __TB_empty [ -1 0 0 0 ] [ -0 -1 -0 0 ] 0 1 1\n"
			"( 64 64 0 ) ( 64 65 0 ) ( 65 64 0 ) __TB_empty [ 1 0 0 0 ] [ 0 -1 0 0 ] 0 1 1\n"
			"( 64 192 16 ) ( 65 192 16 ) ( 64 192 17 ) __TB_empty [ -1 0 0 0 ] [ 0 0 -1 0 ] 0 1 1\n"
			"( 256 64 16 ) ( 256 64 17 ) ( 256 65 16 ) __TB_empty [ 0 1 0 0 ] [ 0 0 -1 0 ] 0 1 1\n"
			"}\n"
			"}"
		),
		"",
		FMAPEntity{
			FMAPParser::MAPProperties(
				"\"classname\" \"worldspawn\"\n"
				"\"mapversion\" \"220\"\n"
				"\"_tb_textures\" \"textures/base\""
			).Value,
			{
				FMAPParser::MAPBrush(
					"{\n"
					"( -240 -64 -16 ) ( -240 -63 -16 ) ( -240 -64 -15 ) __TB_empty [ 0 -1 0 0 ] [ -0 -0 -1 0 ] 0 1 1\n"
					"( -64 -192 -16 ) ( -64 -192 -15 ) ( -63 -192 -16 ) __TB_empty [ 1 0 -0 0 ] [ 0 -0 -1 0 ] 0 1 1\n"
					"( -64 -64 -16 ) ( -63 -64 -16 ) ( -64 -63 -16 ) __TB_empty [ -1 0 0 0 ] [ -0 -1 -0 0 ] 0 1 1\n"
					"( 64 64 0 ) ( 64 65 0 ) ( 65 64 0 ) __TB_empty [ 1 0 0 0 ] [ 0 -1 0 0 ] 0 1 1\n"
					"( 64 192 16 ) ( 65 192 16 ) ( 64 192 17 ) __TB_empty [ -1 0 0 0 ] [ 0 0 -1 0 ] 0 1 1\n"
					"( 256 64 16 ) ( 256 64 17 ) ( 256 65 16 ) __TB_empty [ 0 1 0 0 ] [ 0 0 -1 0 ] 0 1 1\n"
					"}"
				).Value
			}
		}
	);

	return true;
}

IMPLEMENT_CUSTOM_SIMPLE_AUTOMATION_TEST(
	MAPParser_MAPMap,
	FAutomationTestParser,
	"UnrealMAP.MAPParser.MAPMap",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter
)

bool MAPParser_MAPMap::RunTest(const FString& Parameters)
{
	TestEqualParsed<FString, FMAPMap>(
		FMAPParser::MAPMap(
			"// Game: Unreal\n"
			"// Format: Valve\n"
			"// entity 0\n"
			"{\n"
			"\"classname\" \"worldspawn\"\n"
			"\"mapversion\" \"220\"\n"
			"\"_tb_textures\" \"textures/base\"\n"
			"// brush 0\n"
			"{\n"
			"( -240 -64 -16 ) ( -240 -63 -16 ) ( -240 -64 -15 ) __TB_empty [ 0 -1 0 0 ] [ -0 -0 -1 0 ] 0 1 1\n"
			"( -64 -192 -16 ) ( -64 -192 -15 ) ( -63 -192 -16 ) __TB_empty [ 1 0 -0 0 ] [ 0 -0 -1 0 ] 0 1 1\n"
			"( -64 -64 -16 ) ( -63 -64 -16 ) ( -64 -63 -16 ) __TB_empty [ -1 0 0 0 ] [ -0 -1 -0 0 ] 0 1 1\n"
			"( 64 64 0 ) ( 64 65 0 ) ( 65 64 0 ) __TB_empty [ 1 0 0 0 ] [ 0 -1 0 0 ] 0 1 1\n"
			"( 64 192 16 ) ( 65 192 16 ) ( 64 192 17 ) __TB_empty [ -1 0 0 0 ] [ 0 0 -1 0 ] 0 1 1\n"
			"( 256 64 16 ) ( 256 64 17 ) ( 256 65 16 ) __TB_empty [ 0 1 0 0 ] [ 0 0 -1 0 ] 0 1 1\n"
			"}\n"
			"}\n"
			"// entity 1\n"
			"{\n"
			"\"classname\" \"illusionary\"\n"
			"// brush 0\n"
			"{\n"
			"( 1008 544 0 ) ( 1024 704 0 ) ( 1024 704 128 ) special/skip [ -0.0995037 -0.995037 0 461.697 ] [ 0 0 -1 0 ] 0 1 1\n"
			"( 1008 544 0 ) ( 1024 480 128 ) ( 1024 480 0 ) special/skip [ 0.242536 -0.970143 0 395.818 ] [ 0 0 -1 0 ] 0 1 1\n"
			"( 768 448 -16 ) ( 769 448 -16 ) ( 768 449 -16 ) special/skip [ -1 0 0 160 ] [ 0 -1 0 448 ] 0 1 1\n"
			"( 1024 704 0 ) ( 1024 705 0 ) ( 1025 704 0 ) special/skip [ 1 0 0 -160 ] [ 0 -1 0 448 ] 0 1 1\n"
			"( 1024 704 0 ) ( 1024 704 1 ) ( 1024 705 0 ) special/skip [ 0 1 0 -448 ] [ 0 0 -1 0 ] 0 1 1\n"
			"}\n"
			"}"
		),
		"",
		FMAPMap{
			{
				FMAPParser::MAPEntity(
					"{\n"
					"\"classname\" \"worldspawn\"\n"
					"\"mapversion\" \"220\"\n"
					"\"_tb_textures\" \"textures/base\"\n"
					"// brush 0\n"
					"{\n"
					"( -240 -64 -16 ) ( -240 -63 -16 ) ( -240 -64 -15 ) __TB_empty [ 0 -1 0 0 ] [ -0 -0 -1 0 ] 0 1 1\n"
					"( -64 -192 -16 ) ( -64 -192 -15 ) ( -63 -192 -16 ) __TB_empty [ 1 0 -0 0 ] [ 0 -0 -1 0 ] 0 1 1\n"
					"( -64 -64 -16 ) ( -63 -64 -16 ) ( -64 -63 -16 ) __TB_empty [ -1 0 0 0 ] [ -0 -1 -0 0 ] 0 1 1\n"
					"( 64 64 0 ) ( 64 65 0 ) ( 65 64 0 ) __TB_empty [ 1 0 0 0 ] [ 0 -1 0 0 ] 0 1 1\n"
					"( 64 192 16 ) ( 65 192 16 ) ( 64 192 17 ) __TB_empty [ -1 0 0 0 ] [ 0 0 -1 0 ] 0 1 1\n"
					"( 256 64 16 ) ( 256 64 17 ) ( 256 65 16 ) __TB_empty [ 0 1 0 0 ] [ 0 0 -1 0 ] 0 1 1\n"
					"}\n"
					"}"
				).Value,
				FMAPParser::MAPEntity(
					"{\n"
					"\"classname\" \"illusionary\"\n"
					"// brush 0\n"
					"{\n"
					"( 1008 544 0 ) ( 1024 704 0 ) ( 1024 704 128 ) special/skip [ -0.0995037 -0.995037 0 461.697 ] [ 0 0 -1 0 ] 0 1 1\n"
					"( 1008 544 0 ) ( 1024 480 128 ) ( 1024 480 0 ) special/skip [ 0.242536 -0.970143 0 395.818 ] [ 0 0 -1 0 ] 0 1 1\n"
					"( 768 448 -16 ) ( 769 448 -16 ) ( 768 449 -16 ) special/skip [ -1 0 0 160 ] [ 0 -1 0 448 ] 0 1 1\n"
					"( 1024 704 0 ) ( 1024 705 0 ) ( 1025 704 0 ) special/skip [ 1 0 0 -160 ] [ 0 -1 0 448 ] 0 1 1\n"
					"( 1024 704 0 ) ( 1024 704 1 ) ( 1024 705 0 ) special/skip [ 0 1 0 -448 ] [ 0 0 -1 0 ] 0 1 1\n"
					"}\n"
					"}"
				).Value
			}
		}
	);

	return true;
}
