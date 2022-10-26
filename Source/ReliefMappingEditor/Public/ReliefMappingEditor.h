// Copyright (c) 2022 Ryan DowlingSoka

#pragma once

#include "Modules/ModuleManager.h"

namespace ReliefMappingPaths
{
	static FSoftObjectPath ReliefMapGenerator = FSoftObjectPath("/ReliefMapping/ReliefMapping/MapGenerator/ReliefMapGenerator.ReliefMapGenerator");
	static FString ReliefMapPluginString = FString("ReliefMapping");
	static FName ReliefMapPluginName = FName(ReliefMapPluginString);
}

class FReliefMappingEditorModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
