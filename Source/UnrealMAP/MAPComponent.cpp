﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "MAPComponent.h"

#include "MAPBuilder.h"
#include "MAPCache.h"
#include "MAPGlobals.h"
#include "MAPLog.h"
#include "MAPParser.h"
#include "Engine/StaticMeshActor.h"
#include "Misc/FileHelper.h"


UMAPComponent::UMAPComponent()
{
	Config = UMAPGlobals::GetDefaultConfig();
}

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

FString UMAPComponent::GetAbsoluteSourceFile() const
{
	FString AbsolutePath = FPaths::ProjectDir() / SourceFile.FilePath;
	FPaths::NormalizeFilename(AbsolutePath);
	return AbsolutePath;
}

bool UMAPComponent::ShouldBuild() const
{
	const FString AbsolutePath = GetAbsoluteSourceFile();
	const FString CurrentHash = LexToString(FMD5Hash::HashFile(*AbsolutePath));
	return SourceHash != CurrentHash;
}

// TODO: Rebuild on file changes
void UMAPComponent::BuildMAP()
{
	const FString AbsolutePath = GetAbsoluteSourceFile();

	bLoaded = false;
	for (const auto Actor : SpawnedActors)
	{
		if (!Actor) continue;

		Actor->Destroy();
	}
	SpawnedActors.Reset();
	Cache = NewObject<UMAPCache>(this);

	IPlatformFile& FileManager = FPlatformFileManager::Get().GetPlatformFile();
	FString FileContent;
	if (!FileManager.FileExists(*AbsolutePath) || !FFileHelper::LoadFileToString(
		FileContent,
		*AbsolutePath,
		FFileHelper::EHashOptions::None
	))
	{
		UE_LOG(LogMAP, Warning, TEXT("MAPActor: Can't load '%s'"), *AbsolutePath);
		return;
	}

	try
	{
		Map = FMAPParser::MAPMap(FileContent).Value;
		bLoaded = true;
	}
	catch (const std::exception&)
	{
		UE_LOG(LogMAP, Warning, TEXT("MAPActor: Failed to load '%s'"), *AbsolutePath);
		return;
	}

	UE_LOG(LogMAP, Display, TEXT("MAPActor: Successfully loaded '%s'"), *AbsolutePath);

	for (int i = 0; i < Map.Entities.Num(); ++i)
	{
		SpawnEntity(GetWorld(), Map.Entities[i], i, GetOwner());
	}
	SetWorldScale3D(FVector(-1, 1, 1) * 3.125);

	SourceHash = LexToString(FMD5Hash::HashFile(*AbsolutePath));
}

void UMAPComponent::SpawnBrush(UWorld* World, const FMAPBrush& Brush, const int32 Index, AActor* Parent)
{
	UStaticMesh* Mesh = FMAPBuilder::BrushMesh(Brush, Config, Cache);
	AStaticMeshActor* BrushActor = World->SpawnActor<AStaticMeshActor>();
	SpawnedActors.Add(BrushActor);
#if WITH_EDITOR
	BrushActor->SetActorLabel(FString::Printf(TEXT("Brush%d"), Index));
#endif
	BrushActor->SetMobility(EComponentMobility::Movable);
	BrushActor->AttachToActor(Parent, FAttachmentTransformRules::SnapToTargetIncludingScale);
	BrushActor->GetStaticMeshComponent()->SetStaticMesh(Mesh);
}

void UMAPComponent::SpawnEntity(UWorld* World, const FMAPEntity& Entity, const int32 Index, AActor* Parent)
{
	AActor* EntityActor = World->SpawnActor<AActor>();
	SpawnedActors.Add(EntityActor);
#if WITH_EDITOR
	EntityActor->SetActorLabel(FString::Printf(TEXT("Entity%d"), Index));
#endif

	USceneComponent* RootComponent = NewObject<USceneComponent>(
		EntityActor,
		GetDefaultSceneRootVariableName(),
		RF_Transactional
	);
	RootComponent->Mobility = EComponentMobility::Movable;
	RootComponent->bVisualizeComponent = false;

	EntityActor->SetRootComponent(RootComponent);
	EntityActor->AddInstanceComponent(RootComponent);

	RootComponent->RegisterComponent();

	if (Parent)
	{
		EntityActor->AttachToActor(Parent, FAttachmentTransformRules::SnapToTargetIncludingScale);
	}

	for (int i = 0; i < Entity.Brushes.Num(); ++i)
	{
		SpawnBrush(World, Entity.Brushes[i], i, EntityActor);
	}
}
