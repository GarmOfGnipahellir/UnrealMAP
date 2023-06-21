// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MAPModel.h"

struct FMAPConfig;
class UMAPCache;

struct FMAPVertex
{
	FVector Position;
	FVector2d UV;
};

struct FMAPVertexWinding
{
	FVector UAxis;
	FVector VAxis;
	FVector Center;

	FMAPVertexWinding(const FVector& InUAxis, const FVector& InVAxis, const FVector& InCenter);

	bool operator()(const FMAPVertex& A, const FMAPVertex& B) const;
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

	static bool FaceVertex(
		const FMAPFace& Face,
		UTexture2D* Texture,
		const FMAPPlane& Plane1,
		const FMAPPlane& Plane2,
		const FMAPBrush& Brush,
		FMAPVertex& OutVertex);

	static TArray<FMAPVertex> FilterDuplicates(const TArray<FMAPVertex>& Vertices);

	static void SortVertices(const FMAPFace& Face, TArray<FMAPVertex>& Vertices);

	static UTexture2D* FaceTexture(const FMAPFace& Face, const FMAPConfig& Data, UMAPCache* Cache);
	static UStaticMesh* BrushMesh(const FMAPBrush& Brush, const FMAPConfig& Data, UMAPCache* Cache);
};
