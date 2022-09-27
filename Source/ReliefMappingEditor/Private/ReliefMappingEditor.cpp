// Copyright (c) 2022 Ryan DowlingSoka

#include "ReliefMappingEditor.h"

#include "ReliefMappingEditorContentBrowserExtensions.h"

#define LOCTEXT_NAMESPACE "FReliefMappingEditorModule"

void FReliefMappingEditorModule::StartupModule()
{
	FReliefMappingEditorContentBrowserExtensions::InstallHooks();
}

void FReliefMappingEditorModule::ShutdownModule()
{
	FReliefMappingEditorContentBrowserExtensions::RemoveHooks();
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FReliefMappingEditorModule, ReliefMappingEditor)