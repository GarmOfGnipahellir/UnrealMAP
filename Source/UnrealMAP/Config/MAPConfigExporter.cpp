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
	const FString OutputDir = FPaths::GetPath(FilePath);

	const UMAPConfig* Config = Cast<UMAPConfig>(Object);
	if (!Config)
	{
		UE_LOG(LogMAP, Error, TEXT("MAPConfigExporter: Object is not a MAPConfig."))
		return false;
	}

	if (!ExportGameConfig(Config, OutputDir))
	{
		UE_LOG(LogMAP, Error, TEXT("MAPConfigExporter: Failed to export GameConfig."))
		return false;
	}

	if (!ExportGameEngineProfiles(OutputDir))
	{
		UE_LOG(LogMAP, Error, TEXT("MAPConfigExporter: Failed to export GameEngineProfiles."))
		return false;
	}

	if (!ExportEntitiesDefinition(OutputDir))
	{
		UE_LOG(LogMAP, Error, TEXT("MAPConfigExporter: Failed to export EntitiesDefinition."))
		return false;
	}

	return true;
}

bool UMAPConfigExporter::ExportGameConfig(const UMAPConfig* Config, const FString& OutputDir) const
{
	const FString ProjectName = FApp::GetProjectName();

	FJsonDomBuilder::FObject GameConfig;
	GameConfig.Set("version", 5);
	GameConfig.Set("name", ProjectName);

	FJsonDomBuilder::FArray FileFormats;
	FileFormats.Add(
		FJsonDomBuilder::FObject()
		.Set("format", "Valve")
		.Set("initialmap", "initial_valve.map")
	);
	GameConfig.Set("fileformats", FileFormats);

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
		.Set("root", Config->GetTextureRootPlatform())
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
	Entities.Set(
		"definitions",
		FJsonDomBuilder::FArray()
		.Add(FString("Entities.fgd"))
	);
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

	const FString FilePath = OutputDir / "GameConfig.cfg";
	if (!FFileHelper::SaveStringToFile(GameConfig.ToString(), *FilePath))
	{
		UE_LOG(LogMAP, Error, TEXT("MAPConfigExporter: Failed to save file '%s'."), *FilePath)
		return false;
	}

	return true;
}

bool UMAPConfigExporter::ExportGameEngineProfiles(const FString& OutputDir) const
{
	const FString ProjectName = FApp::GetProjectName();
	const FString ProjectFilePath = FPaths::ConvertRelativePathToFull(FPaths::GetProjectFilePath());
	FString EditorBinaryPath;
#if WITH_EDITOR
	EditorBinaryPath = FUnrealEdMisc::Get().GetProjectEditorBinaryPath();
#else
	UE_LOG(LogMAP, Warning, TEXT("MAPConfigExporter: Can't get editor binary path in non editor build."), *FilePath)
#endif

	FStringFormatNamedArguments Args;
	Args.Add(TEXT("ProjectFile"), ProjectFilePath);
	const FString Parameters = FString::Format(TEXT("{ProjectFile}"), Args);

	FJsonDomBuilder::FObject GameEngineProfiles;
	GameEngineProfiles.Set("version", 1);

	FJsonDomBuilder::FArray Profiles;
	Profiles.Add(
		FJsonDomBuilder::FObject()
		.Set("name", ProjectName)
		.Set("parameters", Parameters)
		.Set("path", EditorBinaryPath)
	);
	GameEngineProfiles.Set("profiles", Profiles);

	const FString FilePath = OutputDir / "GameEngineProfiles.cfg";
	if (!FFileHelper::SaveStringToFile(GameEngineProfiles.ToString(), *FilePath))
	{
		UE_LOG(LogMAP, Error, TEXT("MAPConfigExporter: Failed to save file '%s'."), *FilePath)
		return false;
	}

	return true;
}

bool UMAPConfigExporter::ExportEntitiesDefinition(const FString& OutputDir) const
{
	const FString FilePath = OutputDir / "Entities.fgd";
	if (!FFileHelper::SaveStringToFile(TEXT(""), *FilePath))
	{
		UE_LOG(LogMAP, Error, TEXT("MAPConfigExporter: Failed to save file '%s'."), *FilePath)
		return false;
	}
	return true;
}

FString UMAPConfigExporter::GetFilePath() const
{
	return ExportTask != nullptr ? ExportTask->Filename : CurrentFilename;
}
