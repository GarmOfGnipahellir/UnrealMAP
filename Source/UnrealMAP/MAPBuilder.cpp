﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "MAPBuilder.h"

#include "MaterialDomain.h"
#include "MeshDescription.h"
#include "StaticMeshAttributes.h"

FMAPVertexWinding::FMAPVertexWinding(
	const FVector& InUAxis,
	const FVector& InVAxis,
	const FVector& InCenter) : UAxis(InUAxis), VAxis(InVAxis), Center(InCenter)
{
}

FORCEINLINE bool FMAPVertexWinding::operator()(const FVector& A, const FVector& B) const
{
	const auto Lhs = A - Center;
	const auto Rhs = B - Center;

	const auto LhsPu = Lhs | UAxis;
	const auto LhsPv = Lhs | VAxis;

	const auto RhsPu = Rhs | UAxis;
	const auto RhsPv = Rhs | VAxis;

	const auto LhsAngle = FMath::Atan2(LhsPv, LhsPu);
	const auto RhsAngle = FMath::Atan2(RhsPv, RhsPu);

	return RhsAngle < LhsAngle;
}

bool FMAPBuilder::PlaneIntersection(
	const FMAPPlane& Plane,
	const FMAPPlane& Plane1,
	const FMAPPlane& Plane2,
	FVector& OutLocation)
{
	const auto N0 = Plane.GetNormal();
	const auto N1 = Plane1.GetNormal();
	const auto N2 = Plane2.GetNormal();

	const auto Denom = (N0 ^ N1) | N2;
	if (FMath::Abs(Denom) < DOUBLE_KINDA_SMALL_NUMBER) return false;

	const auto D0 = Plane.GetDistance();
	const auto D1 = Plane1.GetDistance();
	const auto D2 = Plane2.GetDistance();

	OutLocation = (D0 * (N1 ^ N2) + D1 * (N2 ^ N0) + D2 * (N0 ^ N1)) / Denom;
	return true;
}

bool FMAPBuilder::LocationInHull(const FVector& Location, const FMAPBrush& Brush)
{
	for (const FMAPFace& Face : Brush.Faces)
	{
		const auto Proj = Face.Plane.GetNormal() | Location;
		const auto Dist = Face.Plane.GetDistance();
		if (Proj > Dist && Proj - Dist > DOUBLE_KINDA_SMALL_NUMBER)
		{
			return false;
		}
	}
	return true;
}

bool FMAPBuilder::PlaneVertex(
	const FMAPPlane& Plane,
	const FMAPPlane& Plane1,
	const FMAPPlane& Plane2,
	const FMAPBrush& Brush,
	FVector& OutLocation)
{
	if (!PlaneIntersection(Plane, Plane1, Plane2, OutLocation))
	{
		return false;
	}

	if (!LocationInHull(OutLocation, Brush))
	{
		return false;
	}

	return true;
}

TArray<FVector> FMAPBuilder::FilterDuplicates(const TArray<FVector>& Vertices)
{
	TArray<FVector> Result;
	for (int i = 0; i < Vertices.Num(); ++i)
	{
		bool bUnique = true;

		for (int j = i + 1; j < Vertices.Num(); ++j)
		{
			if (i == j) continue;

			if (Vertices[i].Equals(Vertices[j]))
			{
				bUnique = false;
				break;
			}
		}

		if (bUnique)
		{
			Result.Add(Vertices[i]);
		}
	}
	return Result;
}

void FMAPBuilder::SortVertices(const FMAPFace& Face, TArray<FVector>& Vertices)
{
	const auto UAxis = (Face.Plane.P1 - Face.Plane.P0).GetSafeNormal();
	const auto VAxis = Face.Plane.GetNormal() ^ UAxis;
	FVector Center;
	for (const auto Vertex : Vertices)
	{
		Center += Vertex;
	}
	Center /= Vertices.Num();

	Vertices.Sort(FMAPVertexWinding(UAxis, VAxis, Center));
}

UStaticMesh* FMAPBuilder::BrushMesh(const FMAPBrush& Brush)
{
	UStaticMesh* Mesh = NewObject<UStaticMesh>();
	Mesh->GetStaticMaterials().Add(UMaterial::GetDefaultMaterial(MD_Surface));
	Mesh->AddSourceModel();
	FMeshDescription* Desc = Mesh->CreateMeshDescription(0);
	Desc->CreatePolygonGroup();
	FStaticMeshAttributes Attributes(*Desc);
	const TVertexAttributesRef<FVector3f> VertexPositions = Attributes.GetVertexPositions();
	const TVertexInstanceAttributesRef<FVector3f> VertexInstanceNormals = Attributes.GetVertexInstanceNormals();

	for (const FMAPFace& Face : Brush.Faces)
	{
		TArray<FVector> FaceVertices;

		for (const FMAPFace& Face1 : Brush.Faces)
		{
			for (const FMAPFace& Face2 : Brush.Faces)
			{
				FVector VertexLocation;
				if (PlaneVertex(Face.Plane, Face1.Plane, Face2.Plane, Brush, VertexLocation))
				{
					FaceVertices.Add(VertexLocation);
				}
			}
		}

		FaceVertices = FilterDuplicates(FaceVertices);
		SortVertices(Face, FaceVertices);

		TArray<FVertexInstanceID> VertexInstanceIDs;
		for (const FVector& Vertex : FaceVertices)
		{
			const FVertexID VertexID = Desc->CreateVertex();
			VertexPositions[VertexID] = FVector3f(Vertex);
			const FVertexInstanceID VertexInstanceID = Desc->CreateVertexInstance(VertexID);
			VertexInstanceNormals[VertexInstanceID] = FVector3f(Face.Plane.GetNormal());
			VertexInstanceIDs.Add(VertexInstanceID);
		}

		Desc->CreatePolygon(FPolygonGroupID(0), VertexInstanceIDs);
	}

	Mesh->CommitMeshDescription(0);

	Mesh->Build(false);

	return Mesh;
}