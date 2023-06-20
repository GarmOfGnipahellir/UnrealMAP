// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MAPModel.h"

struct FMAPVertexWinding
{
	FVector UAxis;
	FVector VAxis;
	FVector Center;

	FMAPVertexWinding(const FVector& InUAxis, const FVector& InVAxis, const FVector& InCenter);

	bool operator()(const FVector& A, const FVector& B) const;
};

/**
 * 
 */
class UNREALMAP_API FMAPBuilder
{
public:
	static bool PlaneIntersection(
		const FMAPPlane& Plane,
		const FMAPPlane& Plane1,
		const FMAPPlane& Plane2,
		FVector& OutLocation);

	static bool LocationInHull(const FVector& Location, const FMAPBrush& Brush);

	static bool PlaneVertex(
		const FMAPPlane& Plane,
		const FMAPPlane& Plane1,
		const FMAPPlane& Plane2,
		const FMAPBrush& Brush,
		FVector& OutLocation);

	static TArray<FVector> FilterDuplicates(const TArray<FVector>& Vertices);

	static void SortVertices(const FMAPFace& Face, TArray<FVector>& Vertices);

	static UStaticMesh* BrushMesh(const FMAPBrush& Brush);
};
