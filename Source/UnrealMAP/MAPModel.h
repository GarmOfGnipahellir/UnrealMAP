// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

struct FMAPPlane
{
	FVector P0;
	FVector P1;
	FVector P2;

	FVector GetNormal() const;
	double GetDistance() const;

	bool operator==(const FMAPPlane& Other) const;
	bool operator!=(const FMAPPlane& Other) const;
};

struct FMAPFace
{
	FMAPPlane Plane;
	FString Texture;
	FVector U;
	FVector V;
	FVector2d Offset;
	double Rotation;
	FVector2d Scale;

	bool operator==(const FMAPFace& Other) const;
	bool operator!=(const FMAPFace& Other) const;
};

struct FMAPBrush
{
	TArray<FMAPFace> Faces;

	bool operator==(const FMAPBrush& Other) const;
	bool operator!=(const FMAPBrush& Other) const;
};

struct FMAPEntity
{
	TMap<FString, FString> Properties;
	TArray<FMAPBrush> Brushes;

	bool operator==(const FMAPEntity& Other) const;
	bool operator!=(const FMAPEntity& Other) const;
};

struct FMAPGroup
{
	FString Name;
	TArray<FMAPGroup> SubGroups;
	TArray<FMAPEntity> Entities;
	TArray<FMAPBrush> Brushes;
};

struct FMAPMap
{
	TArray<FMAPEntity> Entities;

	bool operator==(const FMAPMap& Other) const;
	bool operator!=(const FMAPMap& Other) const;
};
