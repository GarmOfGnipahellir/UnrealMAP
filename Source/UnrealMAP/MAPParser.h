// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

struct FMAPPlane;
struct FMAPFace;
struct FMAPBrush;
struct FMAPEntity;
struct FMAPMap;

/**
 * 
 */
class UNREALMAP_API FMAPParser
{
public:
	static TTuple<FString, FString> Tag(const FString& Tag, const FString& Input);
	static TTuple<FString, FString> NotChar(const wchar_t& Char, const FString& Input, bool bOneOrMore = false);
	static TTuple<FString, float> Float(const FString& Input);
	static TTuple<FString, FString> Whitespace(const FString& Input, bool bOneOrMore = false);
	static TTuple<FString, FString> NotWhitespace(const FString& Input, bool bOneOrMore = false);
	static TTuple<FString, FVector> Vector(const FString& Input);
	static TTuple<FString, FString> Comment(const FString& Input);
	static TTuple<FString, TArray<FString>> Comments(const FString& Input);
	static FString Ignore(const FString& Input);

	static TTuple<FString, FMAPPlane> MAPPlane(const FString& Input);
	static TTuple<FString, FMAPFace> MAPFace(const FString& Input);
	static TTuple<FString, FMAPBrush> MAPBrush(const FString& Input);
	static TTuple<FString, TTuple<FString, FString>> MAPProperty(const FString& Input);
	static TTuple<FString, TMap<FString, FString>> MAPProperties(const FString& Input);
	static TTuple<FString, FMAPEntity> MAPEntity(const FString& Input);
	static TTuple<FString, FMAPMap> MAPMap(const FString& Input);
};
