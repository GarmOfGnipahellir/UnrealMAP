// Fill out your copyright notice in the Description page of Project Settings.


#include "MAPModel.h"

FVector FMAPPlane::GetNormal() const
{
	return ((P2 - P0) ^ (P1 - P0)).GetSafeNormal();
}

double FMAPPlane::GetDistance() const
{
	return GetNormal() | P0;
}

FORCEINLINE bool FMAPPlane::operator==(const FMAPPlane& Other) const
{
	return P0 == Other.P0 && P1 == Other.P1 && P2 == Other.P2;
}

FORCEINLINE bool FMAPPlane::operator!=(const FMAPPlane& Other) const
{
	return P0 != Other.P0 || P1 != Other.P1 || P2 != Other.P2;
}

bool FMAPFace::operator==(const FMAPFace& Other) const
{
	return Plane == Other.Plane &&
		Texture == Other.Texture &&
		U == Other.U &&
		V == Other.V &&
		Offset == Other.Offset &&
		Rotation == Other.Rotation &&
		Scale == Other.Scale;
}

bool FMAPFace::operator!=(const FMAPFace& Other) const
{
	return Plane != Other.Plane ||
		Texture != Other.Texture ||
		U != Other.U ||
		V != Other.V ||
		Offset != Other.Offset ||
		Rotation != Other.Rotation ||
		Scale != Other.Scale;
}

bool FMAPBrush::operator==(const FMAPBrush& Other) const
{
	return Faces == Other.Faces;
}

bool FMAPBrush::operator!=(const FMAPBrush& Other) const
{
	return Faces != Other.Faces;
}

bool FMAPEntity::GetProperty(const FString& Key, FString& OutValue) const
{
	if (const auto Value = Properties.Find(Key))
	{
		OutValue = *Value;
		return true;
	}
	return false;
}

bool FMAPEntity::GetPropertyAsVector(const FString& Key, FVector& OutValue) const
{
	FString TextValue;
	if (GetProperty(Key, TextValue))
	{
		TArray<FString> Parts;
		TextValue.ParseIntoArrayWS(Parts);
		if (Parts.Num() != 3) return false;
		for (const FString& Part : Parts)
		{
			if (!Part.IsNumeric()) return false;
		}
		OutValue = FVector(FCString::Atof(*Parts[0]), FCString::Atof(*Parts[1]), FCString::Atof(*Parts[2]));
		return true;
	}
	return false;
}

bool FMAPEntity::operator==(const FMAPEntity& Other) const
{
	return Properties.OrderIndependentCompareEqual(Other.Properties) && Brushes == Other.Brushes;
}

bool FMAPEntity::operator!=(const FMAPEntity& Other) const
{
	return !Properties.OrderIndependentCompareEqual(Other.Properties) || Brushes != Other.Brushes;
}

bool FMAPMap::operator==(const FMAPMap& Other) const
{
	return Entities == Other.Entities;
}

bool FMAPMap::operator!=(const FMAPMap& Other) const
{
	return Entities != Other.Entities;
}
