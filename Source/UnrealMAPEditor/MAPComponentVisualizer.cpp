// Fill out your copyright notice in the Description page of Project Settings.


#include "MAPComponentVisualizer.h"

#include "CanvasTypes.h"
#include "UnrealMAP/MAPModel.h"
#include "UnrealMAP/MAPBuilder.h"
#include "UnrealMAP/MAPComponent.h"


void FMAPComponentVisualizer::DrawVisualization(
	const UActorComponent* Component,
	const FSceneView* View,
	FPrimitiveDrawInterface* PDI)
{
	const UMAPComponent* MAPComponent = Cast<const UMAPComponent>(Component);
	if (!ensure(MAPComponent)) return;
	if (!MAPComponent->bLoaded) return;

	for (const FMAPEntity& Entity : MAPComponent->MAPMap.Entities)
	{
		for (const FMAPBrush& Brush : Entity.Brushes)
		{
			TArray<FDynamicMeshVertex> BrushVertices;
			
			for (const FMAPFace& Face : Brush.Faces)
			{
				TArray<FVector> FaceVertices;

				for (const FMAPFace& Face1 : Brush.Faces)
				{
					for (const FMAPFace& Face2 : Brush.Faces)
					{
						FVector VertexLocation;
						if (FMAPBuilder::PlaneVertex(Face.Plane, Face1.Plane, Face2.Plane, Brush, VertexLocation))
						{
							FaceVertices.Add(VertexLocation);
							VertexLocation = MAPComponent->GetComponentTransform().TransformPosition(VertexLocation);
							PDI->DrawPoint(VertexLocation, FColor::White, 16.0f, SDPG_World);
						}
					}
				}

				FaceVertices = FMAPBuilder::FilterDuplicates(FaceVertices);
				FMAPBuilder::SortVertices(Face, FaceVertices);

				for (int i = 0; i < FaceVertices.Num(); ++i)
				{
					PDI->DrawLine(
						FaceVertices[i],
						FaceVertices[(i + 1) % FaceVertices.Num()],
						FColor::White,
						SDPG_World
					);
				}
			}
		}
	}
}
