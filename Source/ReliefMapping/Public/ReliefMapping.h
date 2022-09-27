// Copyright (c) 2022 Ryan DowlingSoka

#pragma once

#include "Modules/ModuleManager.h"

class FReliefMappingModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
