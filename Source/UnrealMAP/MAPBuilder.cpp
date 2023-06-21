// Fill out your copyright notice in the Description page of Project Settings.


#include "MAPBuilder.h"

#include "ImageUtils.h"
#include "MAPCache.h"
#include "MAPConfig.h"
#include "MaterialDomain.h"
#include "MeshDescription.h"
#include "StaticMeshAttributes.h"
#include "PhysicsEngine/BodySetup.h"

FMAPVertexWinding::FMAPVertexWinding(
	const FVector& InUAxis,
	const FVector& InVAxis,
	const FVector& InCenter) : UAxis(InUAxis), VAxis(InVAxis), Center(InCenter)
{
}

FORCEINLINE bool FMAPVertexWinding::operator()(const FMAPVertex& A, const FMAPVertex& B) const
{
	const auto Lhs = A.Position - Center;
	const auto Rhs = B.Position - Center;

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

bool FMAPBuilder::FaceVertex(
	const FMAPFace& Face,
	UTexture2D* Texture,
	const FMAPPlane& Plane1,
	const FMAPPlane& Plane2,
	const FMAPBrush& Brush,
	FMAPVertex& OutVertex)
{
	FVector Position;
	if (!PlaneIntersection(Face.Plane, Plane1, Plane2, Position))
	{
		return false;
	}
	if (!LocationInHull(Position, Brush))
	{
		return false;
	}

	const FVector UAxis = Face.U / Face.Scale.X;
	const FVector VAxis = Face.V / Face.Scale.Y;
	FVector2d UV = FVector2d(Position | UAxis, Position | VAxis) + Face.Offset;
	if (Texture)
	{
		UV /= FVector2d(Texture->GetSizeX(), Texture->GetSizeY());
	}

	OutVertex = FMAPVertex{Position, UV};
	return true;
}

TArray<FMAPVertex> FMAPBuilder::FilterDuplicates(const TArray<FMAPVertex>& Vertices)
{
	TArray<FMAPVertex> Result;
	for (int i = 0; i < Vertices.Num(); ++i)
	{
		bool bUnique = true;

		for (int j = i + 1; j < Vertices.Num(); ++j)
		{
			if (i == j) continue;

			if (Vertices[i].Position.Equals(Vertices[j].Position))
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

void FMAPBuilder::SortVertices(const FMAPFace& Face, TArray<FMAPVertex>& Vertices)
{
	const auto UAxis = (Face.Plane.P1 - Face.Plane.P0).GetSafeNormal();
	const auto VAxis = Face.Plane.GetNormal() ^ UAxis;
	FVector Center;
	for (const auto Vertex : Vertices)
	{
		Center += Vertex.Position;
	}
	Center /= Vertices.Num();

	Vertices.Sort(FMAPVertexWinding(UAxis, VAxis, Center));
}

UTexture2D* FMAPBuilder::FaceTexture(const FMAPFace& Face, const FMAPConfig& Data, UMAPCache* Cache)
{
	if (const auto Texture = Cache->Textures.Find(Face.Texture))
	{
		return *Texture;
	}

	const FString AbsolutePath = Data.TextureRoot.Path / Face.Texture + ".png";
	UTexture2D* Texture = FImageUtils::ImportFileAsTexture2D(AbsolutePath);
	if (!Texture) return nullptr;

	Cache->Textures.Add(Face.Texture, Texture);
	return Texture;
}

UStaticMesh* FMAPBuilder::BrushMesh(const FMAPBrush& Brush, const FMAPConfig& Data, UMAPCache* Cache)
{
	UStaticMesh* Mesh = NewObject<UStaticMesh>();
	Mesh->GetStaticMaterials().Add(UMaterial::GetDefaultMaterial(MD_Surface));
	Mesh->AddSourceModel();
	FMeshDescription* Desc = Mesh->CreateMeshDescription(0);
	Desc->CreatePolygonGroup();
	FStaticMeshAttributes Attributes(*Desc);
	const TVertexAttributesRef<FVector3f> VertexPositions = Attributes.GetVertexPositions();
	const TVertexInstanceAttributesRef<FVector3f> VertexInstanceNormals = Attributes.GetVertexInstanceNormals();
	const TVertexInstanceAttributesRef<FVector2f> VertexInstanceUVs = Attributes.GetVertexInstanceUVs();

	TArray<FPlane> Planes;
	TMap<FString, FPolygonGroupID> PolyGroups;
	for (const FMAPFace& Face : Brush.Faces)
	{
		TArray<FMAPVertex> FaceVertices;

		UTexture2D* Texture = FaceTexture(Face, Data, Cache);
		UMaterialInterface* Material;
		if (Data.DefaultMaterial)
		{
			UMaterialInstanceDynamic* MID = UMaterialInstanceDynamic::Create(
				Data.DefaultMaterial,
				GetTransientPackage()
			);
			MID->SetTextureParameterValue("BaseColor", Texture);
			Material = MID;
		}
		else
		{
			Material = UMaterial::GetDefaultMaterial(MD_Surface);
		}
		Mesh->GetStaticMaterials().Add(Material);

		FPolygonGroupID PolyGroupID;
		if (auto PolyGroupIDPtr = PolyGroups.Find(Face.Texture))
		{
			PolyGroupID = *PolyGroupIDPtr;
		}
		else
		{
			PolyGroupID = Desc->CreatePolygonGroup();
			PolyGroups.Add(Face.Texture, PolyGroupID);
		}

		for (const FMAPFace& Face1 : Brush.Faces)
		{
			for (const FMAPFace& Face2 : Brush.Faces)
			{
				FMAPVertex Vertex;
				if (FaceVertex(Face, Texture, Face1.Plane, Face2.Plane, Brush, Vertex))
				{
					FaceVertices.Add(Vertex);
				}
			}
		}

		FaceVertices = FilterDuplicates(FaceVertices);
		SortVertices(Face, FaceVertices);

		FVector FaceNormal = Face.Plane.GetNormal();

		TArray<FVertexInstanceID> VertexInstanceIDs;
		for (const FMAPVertex& Vertex : FaceVertices)
		{
			const FVertexID VertexID = Desc->CreateVertex();
			VertexPositions[VertexID] = FVector3f(Vertex.Position);
			const FVertexInstanceID VertexInstanceID = Desc->CreateVertexInstance(VertexID);
			VertexInstanceNormals[VertexInstanceID] = FVector3f(FaceNormal);
			VertexInstanceUVs[VertexInstanceID] = FVector2f(Vertex.UV);
			VertexInstanceIDs.Add(VertexInstanceID);
		}

		Desc->CreatePolygon(PolyGroupID, VertexInstanceIDs);
		Planes.Add(FPlane(FaceNormal, Face.Plane.GetDistance()));
	}

	Mesh->CommitMeshDescription(0);

	FKConvexElem ConvexElem;
	ConvexElem.HullFromPlanes(Planes, {});
	Mesh->CreateBodySetup();
	Mesh->GetBodySetup()->AggGeom.ConvexElems.Add(ConvexElem);

	Mesh->Build(false);

	return Mesh;
}
