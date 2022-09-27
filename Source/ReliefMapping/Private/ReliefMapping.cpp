// Copyright Epic Games, Inc. All Rights Reserved.

#include "ReliefMapping.h"

#include "Interfaces/IPluginManager.h"
#include "ShaderCore.h"

#define LOCTEXT_NAMESPACE "FReliefMappingModule"

void FReliefMappingModule::StartupModule()
{
	// Add shader directory
	const TSharedPtr<IPlugin> ReliefMappingPlugin = IPluginManager::Get().FindPlugin(TEXT("ReliefMapping"));
	if (ReliefMappingPlugin.IsValid())
	{
		const FString RealShaderDirectory = ReliefMappingPlugin->GetBaseDir() + TEXT("/Shaders/");
		const FString VirtualShaderDirectory = TEXT("/Plugins/ReliefMapping");
		AddShaderSourceDirectoryMapping(VirtualShaderDirectory, RealShaderDirectory);
	}
}

void FReliefMappingModule::ShutdownModule()
{
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FReliefMappingModule, ReliefMapping)