// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MAPModel.h"
#include "MAPComponent.generated.h"


UCLASS(ClassGroup=(MAP), meta=(BlueprintSpawnableComponent))
class UNREALMAP_API UMAPComponent : public USceneComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(Category="MAP", EditAnywhere, meta=(FilePathFilter="MAP files (*.map)|*.map", RelativeToGameDir))
	FFilePath SourceFile;

	FMAPMap MAPMap;
	bool bLoaded = false;

	UMAPComponent();

#if WITH_EDITORONLY_DATA
	virtual void PostLoad() override;
#endif
	virtual void PostInitProperties() override;
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

	void BuildMAP();

	void SpawnBrush(UWorld* World, const FMAPBrush& Brush, int32 Index, AActor* Parent = nullptr);
	void SpawnEntity(UWorld* World, const FMAPEntity& Entity, int32 Index, AActor* Parent = nullptr);

private:
	UPROPERTY()
	TArray<TObjectPtr<AActor>> SpawnedActors;
};
