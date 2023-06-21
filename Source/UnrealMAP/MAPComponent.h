// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MAPConfig.h"
#include "MAPModel.h"
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
	FMAPConfig Data;

	UPROPERTY(Category="MAP", VisibleAnywhere)
	TObjectPtr<UMAPCache> Cache;

	UPROPERTY()
	bool bLoaded = false;
	
	FMAPMap Map;

	UMAPComponent();
	
	virtual void PostInitProperties() override;
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

	FString GetAbsoluteSourceFile() const;
	bool ShouldBuild() const;
	void BuildMAP();

	void SpawnBrush(UWorld* World, const FMAPBrush& Brush, int32 Index, AActor* Parent = nullptr);
	void SpawnEntity(UWorld* World, const FMAPEntity& Entity, int32 Index, AActor* Parent = nullptr);

private:
	UPROPERTY()
	TArray<TObjectPtr<AActor>> SpawnedActors;
};
