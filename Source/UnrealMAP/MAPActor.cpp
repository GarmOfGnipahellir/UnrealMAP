// Fill out your copyright notice in the Description page of Project Settings.


#include "MAPActor.h"

#include "MAPBuilder.h"
#include "MAPComponent.h"
#include "Engine/StaticMeshActor.h"


AMAPActor::AMAPActor()
{
	MAPComponent = CreateDefaultSubobject<UMAPComponent>(TEXT("MAPComponent0"));
	RootComponent = MAPComponent;
}
