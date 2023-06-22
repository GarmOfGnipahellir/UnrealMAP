// Fill out your copyright notice in the Description page of Project Settings.


#include "MAPConfig.h"

#include "JsonDomBuilder.h"
#include "MAPConfigExporter.h"

FString UMAPConfig::GetTextureRootPlatform() const
{
	FString Path = TextureRoot.Path;
	if (Path.StartsWith("/Game"))
	{
		return Path.Replace(TEXT("/Game"), TEXT("Content"));
	}
	// TODO: This only works for project plugins...
	TArray<FString> Parts;
	Path.ParseIntoArray(Parts, TEXT("/"));
	Parts.Insert("Content", 1);
	Parts.Insert("Plugins", 0);
	return FString::Join(Parts, TEXT("/"));
}

FName UMAPConfig::GetExporterName()
{
	return UMAPConfigExporter::StaticClass()->GetFName();
}
