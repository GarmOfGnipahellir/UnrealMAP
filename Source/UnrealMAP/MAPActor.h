// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MAPActor.generated.h"

class UMAPComponent;
struct FMAPBrush;
struct FMAPEntity;

UCLASS(ComponentWrapperClass)
class UNREALMAP_API AMAPActor : public AActor
{
	GENERATED_BODY()

public:
	UPROPERTY(Category="MAP", VisibleAnywhere)
	TObjectPtr<UMAPComponent> MAPComponent;

	AMAPActor();

	void SpawnBrush(UWorld* World, const FMAPBrush& Brush, AActor* Parent = nullptr);
	void SpawnEntity(UWorld* World, const FMAPEntity& Entity, AActor* Parent = nullptr);

private:
	TArray<TObjectPtr<AActor>> SpawnedActors;
};
