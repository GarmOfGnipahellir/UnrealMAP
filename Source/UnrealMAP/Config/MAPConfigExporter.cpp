// Fill out your copyright notice in the Description page of Project Settings.


#include "MAPConfigExporter.h"

#include "AssetExportTask.h"
#include "JsonDomBuilder.h"
#include "MAPConfig.h"
#include "Misc/FileHelper.h"
#include "UnrealMAP/MAPLog.h"

UMAPConfigExporter::UMAPConfigExporter() : UExporter()
{
	SupportedClass = UMAPConfig::StaticClass();
	PreferredFormatIndex = 0;

	FormatExtension.Add("cfg");
	FormatDescription.Add("TrenchBroom Game Config");
}

bool UMAPConfigExporter::ExportBinary(
		UObject* Object,
		const TCHAR* Type,
		FArchive& Ar,
		FFeedbackContext* Warn,
		int32 FileIndex,
		uint32 PortFlags)
{
	const FString FilePath = GetFilePath();
	const FString FileName = FPaths::GetCleanFilename(FilePath);
	const FString DirPath = FPaths::GetPath(FilePath);

	const UMAPConfig* Config = Cast<UMAPConfig>(Object);
	if (!Config)
	{
		UE_LOG(LogMAP, Error, TEXT("MAPConfigExporter: Object is not a MAPConfig."))
		return false;
	}

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
		.Set("root", Config->TextureRoot.Path)
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

	if (!FFileHelper::SaveStringToFile(GameConfig.ToString(), *(DirPath / "GameConfig.cfg")))
	{
		UE_LOG(LogMAP, Error, TEXT("MAPConfigExporter: Failed to save file '%s'."), *(DirPath / "GameConfig.cfg"))
		return false;
	}

	return true;
}

FString UMAPConfigExporter::GetFilePath() const
{
	return ExportTask != nullptr ? ExportTask->Filename : CurrentFilename;
}
