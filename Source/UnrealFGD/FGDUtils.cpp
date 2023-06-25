// Fill out your copyright notice in the Description page of Project Settings.


#include "FGDUtils.h"

#include <cwctype>

FString FGDUtils::PascalCaseToSnakeCase(const FString& InString)
{
	FString Result;
	for (const auto Char : InString)
	{
		if (std::iswupper(Char))
		{
			if (!Result.IsEmpty())
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
