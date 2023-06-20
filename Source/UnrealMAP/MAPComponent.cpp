// Fill out your copyright notice in the Description page of Project Settings.


#include "MAPComponent.h"

#include "MAPBuilder.h"
#include "MAPParser.h"
#include "Engine/StaticMeshActor.h"


UMAPComponent::UMAPComponent()
{
}

#if WITH_EDITORONLY_DATA
void UMAPComponent::PostLoad()
{
	Super::PostLoad();

	BuildMAP();
}
#endif

void UMAPComponent::PostInitProperties()
{
	Super::PostInitProperties();

	BuildMAP();
}

void UMAPComponent::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	BuildMAP();
}

void UMAPComponent::BuildMAP()
{
	bLoaded = false;
	for (const auto Actor : SpawnedActors)
	{
		Actor->Destroy();
	}
	SpawnedActors.Reset();

	IPlatformFile& FileManager = FPlatformFileManager::Get().GetPlatformFile();

	const FString AbsolutePath = FPaths::ProjectDir() / SourceFile.FilePath;

	FString FileContent;
	if (!FileManager.FileExists(*AbsolutePath) || !FFileHelper::LoadFileToString(
		FileContent,
		*AbsolutePath,
		FFileHelper::EHashOptions::None
	))
	{
		UE_LOG(LogTemp, Warning, TEXT("MAPActor: Can't load '%s'"), *AbsolutePath);
		return;
	}

	try
	{
		MAPMap = FMAPParser::MAPMap(FileContent).Value;
		bLoaded = true;
	}
	catch (const std::exception&)
	{
		UE_LOG(LogTemp, Warning, TEXT("MAPActor: Failed to load '%s'"), *AbsolutePath);
		return;
	}

	for (const FMAPEntity& Entity : MAPMap.Entities)
	{
		for (const FMAPBrush& Brush : Entity.Brushes)
		{
			SpawnBrush(GetWorld(), Brush, GetOwner());
		}
	}
}

void UMAPComponent::SpawnBrush(UWorld* World, const FMAPBrush& Brush, AActor* Parent)
{
	UStaticMesh* Mesh = FMAPBuilder::BrushMesh(Brush);
	AStaticMeshActor* BrushActor = World->SpawnActor<AStaticMeshActor>();
	BrushActor->AttachToActor(Parent, FAttachmentTransformRules::KeepWorldTransform);
	BrushActor->GetStaticMeshComponent()->SetStaticMesh(Mesh);
	SpawnedActors.Add(BrushActor);
}

void UMAPComponent::SpawnEntity(UWorld* World, const FMAPEntity& Entity, AActor* Parent)
{
}
