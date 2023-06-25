#include "Misc/AutomationTest.h"
#include "UnrealFGD/FGDUtils.h"

DEFINE_SPEC(
	FFGDUtils_Spec,
	"UnrealFGD.FGDUtils",
	EAutomationTestFlags::ProductFilter | EAutomationTestFlags::ApplicationContextMask
)

void FFGDUtils_Spec::Define()
{
	Describe(
		"ParseDouble",
		[this]
		{
			It(
				"should parse scientific notation",
				[this]
				{
					TestEqual(
						"Positive with negative exponent",
						*FGDUtils::ParseDouble("2.222e-16"),
						0.0000000000000002222
					);
					TestEqual(
						"Positive with positive exponent",
						*FGDUtils::ParseDouble("2.222e16"),
						22220000000000000
					);
					TestEqual(
						"Negative with negative exponent",
						*FGDUtils::ParseDouble("-2.222e-16"),
						-0.0000000000000002222
					);
					TestEqual(
						"Negative with positive exponent",
						*FGDUtils::ParseDouble("-2.222e16"),
						-22220000000000000
					);
				}
			);

			It(
				"should parse fraction",
				[this]
				{
					TestEqual("Negative", *FGDUtils::ParseDouble("-2.222"), -2.222);
					TestEqual("Positive", *FGDUtils::ParseDouble("2.222"), 2.222);
				}
			);

			It(
				"should parse integer",
				[this]
				{
					TestEqual("Negative", *FGDUtils::ParseDouble("-42"), -42.0);
					TestEqual("Positive", *FGDUtils::ParseDouble("42"), 42.0);
				}
			);

			It(
				"should fail if not a number",
				[this]
				{
					TestFalse("Not number", FGDUtils::ParseDouble("abc").IsSet());
					TestFalse("Partial number end", FGDUtils::ParseDouble("abc42").IsSet());
					TestFalse("Partial number start", FGDUtils::ParseDouble("42abc").IsSet());
				}
			);
		}
	);

	Describe(
		"ParseInteger",
		[this]
		{
			It(
				"should parse from numeric string",
				[this]
				{
					TestEqual("Negative", *FGDUtils::ParseInteger("-496"), -496);
					TestEqual("Positive", *FGDUtils::ParseInteger("496"), 496);
				}
			);
			
			It(
				"should fail on non-numeric string",
				[this]
				{
					TestFalse("Not number", FGDUtils::ParseInteger("abc").IsSet());
					TestFalse("Partial number start", FGDUtils::ParseInteger("42abc").IsSet());
					TestFalse("Partial number end", FGDUtils::ParseInteger("abc42").IsSet());
				}
			);
		}
	);

	Describe(
		"ParseVector",
		[this]
		{
			It(
				"should parse from 3 space seperated numbers",
				[this]
				{
					TestEqual("Vector", *FGDUtils::ParseVector("42 -12.8 78e-4"), FVector(42, -12.8, 0.0078));
				}
			);

			It(
				"should fail if not 3 space seperated numbers",
				[this]
				{
					TestFalse("One number", FGDUtils::ParseVector("42").IsSet());
					TestFalse("Two numbers", FGDUtils::ParseVector("42 -12.8").IsSet());
				}
			);
		}
	);

	Describe(
		"ParseColor",
		[this]
		{
			It(
				"should parse from 3 space seperated numbers",
				[this]
				{
					TestEqual("Color", *FGDUtils::ParseColor("7 63 255"), FColor(7, 63, 255));
				}
			);

			It(
				"should fail if not 3 space seperated numbers",
				[this]
				{
					TestFalse("One number", FGDUtils::ParseColor("42").IsSet());
					TestFalse("Two numbers", FGDUtils::ParseColor("42 12").IsSet());
				}
			);
		}
	);
}
