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

	for (const FMAPEntity& Entity : MAPComponent->Map.Entities)
	{
		for (const FMAPBrush& Brush : Entity.Brushes)
		{
			TArray<FDynamicMeshVertex> BrushVertices;

			for (const FMAPFace& Face : Brush.Faces)
			{
				TArray<FMAPVertex> FaceVertices;

				for (const FMAPFace& Face1 : Brush.Faces)
				{
					for (const FMAPFace& Face2 : Brush.Faces)
					{
						FMAPVertex Vertex;
						if (FMAPBuilder::FaceVertex(Face, nullptr, Face1.Plane, Face2.Plane, Brush, Vertex))
						{
							FaceVertices.Add(Vertex);
							const FVector VertexLocation = MAPComponent->GetComponentTransform().TransformPosition(
								Vertex.Position
							);
							PDI->DrawPoint(VertexLocation, FColor::White, 16.0f, SDPG_World);
						}
					}
				}

				FaceVertices = FMAPBuilder::FilterDuplicates(FaceVertices);
				FMAPBuilder::SortVertices(Face, FaceVertices);

				for (int i = 0; i < FaceVertices.Num(); ++i)
				{
					const FVector Start = MAPComponent->GetComponentTransform().TransformPosition(
						FaceVertices[i].Position
					);
					const FVector End = MAPComponent->GetComponentTransform().TransformPosition(
						FaceVertices[(i + 1) % FaceVertices.Num()].Position
					);
					PDI->DrawLine(
						Start,
						End,
						FColor::White,
						SDPG_World
					);
				}
			}
		}
	}
}
