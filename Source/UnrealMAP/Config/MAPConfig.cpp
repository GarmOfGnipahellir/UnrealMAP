// Fill out your copyright notice in the Description page of Project Settings.


#include "MAPConfig.h"

#include "JsonDomBuilder.h"
#include "MAPConfigExporter.h"
#include "UnrealMAP/MAPLog.h"


void UMAPConfig::Export()
{
	const FString OutputDir = FPaths::ProjectSavedDir() / "UnrealMAP";
	if (!ExportForTrenchBroom(OutputDir))
	{
		UE_LOG(LogMAP, Warning, TEXT("Failed to export MAPConfig"));
	}
}

FString UMAPConfig::GetTextureRootPlatform() const
{
	return TextureRoot.Path;
}

bool UMAPConfig::ExportForTrenchBroom(const FString& OutputDir) const
{
	FJsonDomBuilder::FObject GameConfig;
	GameConfig.Set("version", 4);

	FJsonDomBuilder::FObject FileSystem;
	FileSystem.Set("searchpath", ".");
	FileSystem.Set(
		"packageformat",
		FJsonDomBuilder::FObject()
		.Set("extension", "zip")
		.Set("format", "zip")
	);
	GameConfig.Set("filesystem", FileSystem);

	FJsonDomBuilder::FObject Textures;
	Textures.Set(
		"package",
		FJsonDomBuilder::FObject()
		.Set("type", "directory")
		.Set("root", TextureRoot.Path)
	);
	Textures.Set(
		"format",
		FJsonDomBuilder::FObject()
		.Set("extension", "png")
		.Set("format", "image")
	);
	Textures.Set("attribute", "_tb_textures");
	GameConfig.Set("textures", Textures);

	FJsonDomBuilder::FObject Entities;
	Entities.Set("definitions", FJsonDomBuilder::FArray());
	Entities.Set("defaultcolor", "0.6 0.6 0.6 1.0");
	GameConfig.Set("entities", Entities);

	FJsonDomBuilder::FObject Tags;
	Tags.Set("brush", FJsonDomBuilder::FArray());
	Tags.Set("brushface", FJsonDomBuilder::FArray());
	GameConfig.Set("tags", Tags);

	FJsonDomBuilder::FObject FaceAttribs;
	FaceAttribs.Set("surfaceflags", FJsonDomBuilder::FArray());
	FaceAttribs.Set("contentflags", FJsonDomBuilder::FArray());
	GameConfig.Set("faceattribs", FaceAttribs);

	UE_LOG(LogMAP, Display, TEXT("GameConfig: %s"), *GameConfig.ToString());

	return true;
}

FName UMAPConfig::GetExporterName()
{
	return UMAPConfigExporter::StaticClass()->GetFName();
}
