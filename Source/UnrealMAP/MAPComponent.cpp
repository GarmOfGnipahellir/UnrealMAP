// Fill out your copyright notice in the Description page of Project Settings.


#include "MAPComponent.h"

#include "DirectoryWatcherModule.h"
#include "IDirectoryWatcher.h"
#include "MAPBuilder.h"
#include "MAPCache.h"
#include "MAPGlobals.h"
#include "MAPLog.h"
#include "MAPParser.h"
#include "AssetRegistry/IAssetRegistry.h"
#include "Config/MAPConfigExporter.h"
#include "Engine/PointLight.h"
#include "Engine/StaticMeshActor.h"
#include "GameFramework/PlayerStart.h"
#include "Misc/FileHelper.h"
#include "UnrealFGD/Types/FGDEntity.h"


UMAPComponent::UMAPComponent()
{
	Config = UMAPGlobals::GetDefaultConfig();
	WatcherDelegateHandle = FDelegateHandle();
}

void UMAPComponent::RebuildMAP()
{
	ForceBuildMAP();
}

#if WITH_EDITOR
void UMAPComponent::PostLoad()
{
	Super::PostLoad();

	SetupWatcher();
}
#endif

void UMAPComponent::PostInitProperties()
{
	Super::PostInitProperties();

	SetupAssetDelegates();
	BuildMAP();
}

void UMAPComponent::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	if (PropertyChangedEvent.GetMemberPropertyName() == GET_MEMBER_NAME_CHECKED(ThisClass, SourceFile))
	{
		SetupWatcher();
		SourceHash.Reset();
	}

	if (PropertyChangedEvent.GetMemberPropertyName() == GET_MEMBER_NAME_CHECKED(ThisClass, Config))
	{
		SetupAssetDelegates();
	}

	BuildMAP();
}

void UMAPComponent::SetupWatcher()
{
	FDirectoryWatcherModule& DirectoryWatcherModule = FModuleManager::LoadModuleChecked<FDirectoryWatcherModule>(
		"DirectoryWatcher"
	);
	DirectoryWatcherModule.Get()->UnregisterDirectoryChangedCallback_Handle(WatchDir, WatcherDelegateHandle);
	WatcherDelegateHandle.Reset();

	if (SourceFile.FilePath.IsEmpty()) return;

	WatchDir = FPaths::GetPath(GetAbsoluteSourceFile());
	DirectoryWatcherModule.Get()->RegisterDirectoryChangedCallback_Handle(
		WatchDir,
		IDirectoryWatcher::FDirectoryChanged::CreateUObject(this, &UMAPComponent::OnDirectoryChanged),
		WatcherDelegateHandle,
		0
	);
	UE_LOG(LogMAP, Display, TEXT("MAPComponent: Setup watcher in '%s'."), *WatchDir);
}

void UMAPComponent::OnDirectoryChanged(const TArray<FFileChangeData>& Data)
{
	for (const FFileChangeData& Change : Data)
	{
		if (Change.Filename == GetAbsoluteSourceFile())
		{
			BuildMAP();
		}
	}
}

void UMAPComponent::SetupAssetDelegates()
{
	IAssetRegistry* AssetRegistry = IAssetRegistry::Get();

	AssetRegistry->OnAssetAdded().RemoveAll(this);
	AssetRegistry->OnAssetRemoved().RemoveAll(this);
	AssetRegistry->OnAssetRenamed().RemoveAll(this);
	AssetRegistry->OnAssetUpdated().RemoveAll(this);

	AssetRegistry->OnAssetAdded().AddUObject(this, &UMAPComponent::OnAssetAdded);
	AssetRegistry->OnAssetRemoved().AddUObject(this, &UMAPComponent::OnAssetRemoved);
	AssetRegistry->OnAssetRenamed().AddUObject(this, &UMAPComponent::OnAssetRenamed);
	AssetRegistry->OnAssetUpdated().AddUObject(this, &UMAPComponent::OnAssetUpdated);
}

void UMAPComponent::OnAssetAdded(const FAssetData& Asset) const
{
	if (!Config) return;

	const FString ObjectPath = Asset.GetObjectPathString();
	if (ObjectPath.StartsWith(Config->TextureRoot.Path))
	{
		UE_LOG(LogMAP, Display, TEXT("MAPComponent: '%s' added."), *ObjectPath);
		if (UMAPConfigExporter::ExportTextureAsset(Config, Asset))
		{
			UE_LOG(LogMAP, Display, TEXT("MAPComponent: Exported '%s' as texture."), *ObjectPath)
		}
	}
}

void UMAPComponent::OnAssetRemoved(const FAssetData& Asset) const
{
	if (!Config) return;

	const FString ObjectPath = Asset.GetObjectPathString();
	if (ObjectPath.StartsWith(Config->TextureRoot.Path))
	{
		UE_LOG(LogMAP, Display, TEXT("MAPComponent: '%s' removed."), *ObjectPath);
		const FString TexturePath = UMAPConfigExporter::GetAssetTexturePath(Config, ObjectPath);
		if (FPlatformFileManager::Get().GetPlatformFile().DeleteFile(*TexturePath))
		{
			UE_LOG(LogMAP, Display, TEXT("MAPComponent: Deleted '%s'."), *TexturePath)
		}
	}
}

void UMAPComponent::OnAssetRenamed(const FAssetData& Asset, const FString& OldName) const
{
	if (!Config) return;

	const FString ObjectPath = Asset.GetObjectPathString();
	if (ObjectPath.StartsWith(Config->TextureRoot.Path))
	{
		UE_LOG(LogMAP, Display, TEXT("MAPComponent: '%s' renamed to '%s'."), *OldName, *ObjectPath);
		const FString TexturePath = UMAPConfigExporter::GetAssetTexturePath(Config, OldName);
		if (FPlatformFileManager::Get().GetPlatformFile().DeleteFile(*TexturePath))
		{
			UE_LOG(LogMAP, Display, TEXT("MAPComponent: Deleted '%s'."), *TexturePath)
		}
	}
}

void UMAPComponent::OnAssetUpdated(const FAssetData& Asset) const
{
	if (!Config) return;

	const FString ObjectPath = Asset.GetObjectPathString();
	if (ObjectPath.StartsWith(Config->TextureRoot.Path))
	{
		UE_LOG(LogMAP, Display, TEXT("MAPComponent: '%s' updated."), *ObjectPath);
		if (UMAPConfigExporter::ExportTextureAsset(Config, Asset))
		{
			UE_LOG(LogMAP, Display, TEXT("MAPComponent: Exported '%s' as texture."), *ObjectPath)
		}
	}
}

FString UMAPComponent::GetAbsoluteSourceFile() const
{
	const FString AbsolutePath = FPaths::ProjectDir() / SourceFile.FilePath;
	return FPaths::ConvertRelativePathToFull(AbsolutePath);
}

bool UMAPComponent::ShouldBuild() const
{
	const FString AbsolutePath = GetAbsoluteSourceFile();
	const FString CurrentHash = LexToString(FMD5Hash::HashFile(*AbsolutePath));
	return SourceHash != CurrentHash;
}

void UMAPComponent::BuildMAP()
{
	if (!ShouldBuild()) return;
	BuildMAPImpl();
}

void UMAPComponent::ForceBuildMAP()
{
	BuildMAPImpl();
}

void UMAPComponent::BuildMAPImpl()
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
	AActor* EntityActor = nullptr;
	FString ClassName;
	if (Entity.GetProperty("classname", ClassName))
	{
		UClass* Class = nullptr;
		for (TObjectIterator<UClass> It; It; ++It)
		{
			if ((*It)->GetName() == ClassName)
			{
				Class = *It;
				break;
			}
		}
		if (Class)
		{
			EntityActor = World->SpawnActor(Class);
		}
	}

	if (!EntityActor)
	{
		EntityActor = World->SpawnActor<AActor>();

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
	}

	SpawnedActors.Add(EntityActor);

#if WITH_EDITOR
	EntityActor->SetActorLabel(FString::Printf(TEXT("Entity%d"), Index));
#endif

	if (Parent)
	{
		EntityActor->AttachToActor(Parent, FAttachmentTransformRules::SnapToTargetIncludingScale);
	}

	FVector Origin;
	if (Entity.GetPropertyAsVector("origin", Origin))
	{
		EntityActor->GetRootComponent()->SetRelativeLocation(Origin);
	}
	
	// TODO: Handle all FGD properties

	for (int i = 0; i < Entity.Brushes.Num(); ++i)
	{
		SpawnBrush(World, Entity.Brushes[i], i, EntityActor);
	}
}
