// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IDirectoryWatcher.h"
#include "MAPModel.h"
#include "Config/MAPConfig.h"
#include "MAPComponent.generated.h"

class UMAPCache;

UCLASS(ClassGroup=(MAP), meta=(BlueprintSpawnableComponent))
class UNREALMAP_API UMAPComponent : public USceneComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(Category="MAP", EditAnywhere, meta=(FilePathFilter="MAP files (*.map)|*.map", RelativeToGameDir))
	FFilePath SourceFile;

	UPROPERTY(Category="MAP", VisibleAnywhere)
	FString SourceHash;

	UPROPERTY(Category="MAP", EditAnywhere)
	TObjectPtr<UMAPConfig> Config;

	UPROPERTY(Category="MAP", VisibleAnywhere)
	TObjectPtr<UMAPCache> Cache;

	UPROPERTY()
	bool bLoaded = false;

	FMAPMap Map;

	UMAPComponent();

#if WITH_EDITOR
	virtual void PostLoad() override;
#endif
	virtual void PostInitProperties() override;
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

	void SetupWatcher();
	void OnDirectoryChanged(const TArray<FFileChangeData>& Data);

	void SetupAssetDelegates();
	void OnAssetAdded(const FAssetData& Asset) const;
	void OnAssetRemoved(const FAssetData& Asset) const;
	void OnAssetRenamed(const FAssetData& Asset, const FString& OldName) const;
	void OnAssetUpdated(const FAssetData& Asset) const;

	FString GetAbsoluteSourceFile() const;
	bool ShouldBuild() const;
	void BuildMAP();
	void ForceBuildMAP();
	void BuildMAPImpl();

	void SpawnBrush(UWorld* World, const FMAPBrush& Brush, int32 Index, AActor* Parent = nullptr);
	void SpawnEntity(UWorld* World, const FMAPEntity& Entity, int32 Index, AActor* Parent = nullptr);

private:
	UPROPERTY()
	TArray<TObjectPtr<AActor>> SpawnedActors;

	FString WatchDir;
	FDelegateHandle WatcherDelegateHandle;
};
