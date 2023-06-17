// Fill out your copyright notice in the Description page of Project Settings.


#include "MAPModel.h"

FVector FMAPPlane::GetNormal() const
{
	return FVector::CrossProduct(
		P1 - P0,
		P2 - P0
	).GetSafeNormal();
}

double FMAPPlane::GetDistance() const
{
	return GetNormal().Dot(P0);
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
