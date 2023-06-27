// Fill out your copyright notice in the Description page of Project Settings.


#include "FGDUtils.h"
#include "Internationalization/Regex.h"

#include <cwctype>

FString FGDUtils::PascalCaseToSnakeCase(const FString& InString)
{
	FString Result;
	for (int i = 0; i < InString.Len(); ++i)
	{
		const auto Char = InString[i];

		// Skip first 'b' on bool properties
		if (Char == 'b' && Result.IsEmpty() && InString.Len() > 1)
		{
			continue;
		}

		if (std::iswupper(Char))
		{
			if (!Result.IsEmpty() && i < InString.Len() - 1 && std::iswlower(InString[i + 1]))
			{
				Result += '_';
			}
			Result += std::towlower(Char);
			continue;
		}
		Result += Char;
	}
	return Result;
}

TOptional<double> FGDUtils::ParseDouble(const FString& InString)
{
	if (InString.IsEmpty()) return TOptional<double>();

	const FRegexPattern Pattern(TEXT("^(-?\\d*\\.?\\d*)(?:e(-?\\d+))?$"));
	FRegexMatcher Matcher(Pattern, InString);
	if (!Matcher.FindNext()) return TOptional<double>();

	const FString CoefficientString = Matcher.GetCaptureGroup(1);
	const FString ExponentString = Matcher.GetCaptureGroup(2);

	if (!CoefficientString.IsNumeric()) return TOptional<double>();
	const double Coefficient = FCString::Atod(*CoefficientString);

	if (!ExponentString.IsNumeric()) return Coefficient;
	const double Exponent = FCString::Atod(*ExponentString);

	return Coefficient * FMath::Pow(10, Exponent);
}

TOptional<int> FGDUtils::ParseInteger(const FString& InString)
{
	if (!InString.IsNumeric()) return TOptional<int>();
	return FCString::Atoi(*InString);
}

TOptional<bool> FGDUtils::ParseBool(const FString& InString)
{
	const TOptional<int> Integer = ParseInteger(InString);
	if (!Integer) return TOptional<bool>();
	return *Integer > 0;
}

TOptional<FVector> FGDUtils::ParseVector(const FString& InString)
{
	if (InString.IsEmpty()) return TOptional<FVector>();

	const FRegexPattern Pattern(TEXT("^(.+)\\s(.+)\\s(.+)$"));
	FRegexMatcher Matcher(Pattern, InString);
	if (!Matcher.FindNext()) return TOptional<FVector>();

	const FString XString = Matcher.GetCaptureGroup(1);
	const TOptional<double> X = ParseDouble(XString);
	if (!X) return TOptional<FVector>();

	const FString YString = Matcher.GetCaptureGroup(2);
	const TOptional<double> Y = ParseDouble(YString);
	if (!Y) return TOptional<FVector>();

	const FString ZString = Matcher.GetCaptureGroup(3);
	const TOptional<double> Z = ParseDouble(ZString);
	if (!Z) return TOptional<FVector>();

	return FVector(*X, *Y, *Z);
}

TOptional<FColor> FGDUtils::ParseColor(const FString& InString)
{
	if (InString.IsEmpty()) return TOptional<FColor>();

	const FRegexPattern Pattern(TEXT("^(\\d+)\\s(\\d+)\\s(\\d+)$"));
	FRegexMatcher Matcher(Pattern, InString);
	if (!Matcher.FindNext()) return TOptional<FColor>();

	const FString RString = Matcher.GetCaptureGroup(1);
	const TOptional<int> R = ParseInteger(RString);
	if (!R) return TOptional<FColor>();

	const FString GString = Matcher.GetCaptureGroup(2);
	const TOptional<int> G = ParseInteger(GString);
	if (!G) return TOptional<FColor>();

	const FString BString = Matcher.GetCaptureGroup(3);
	const TOptional<int> B = ParseInteger(BString);
	if (!B) return TOptional<FColor>();

	return FColor(*R, *G, *B);
}
