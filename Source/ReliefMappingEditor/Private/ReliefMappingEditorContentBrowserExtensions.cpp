// Copyright (c) 2022 Ryan DowlingSoka

#include "ReliefMappingEditorContentBrowserExtensions.h"
#include "ContentBrowserModule.h"
#include "Editor.h"
#include "EditorUtilitySubsystem.h"
#include "EditorUtilityWidget.h"
#include "EditorUtilityWidgetBlueprint.h"
#include "IContentBrowserSingleton.h"
#include "ReliefMappingEditor.h"
#include "ReliefMappingGeneratorInterface.h"
#include "ReliefMapUserData.h"
#include "Engine/Texture2D.h"
#include "Framework/Commands/UIAction.h"
#include "Framework/MultiBox/MultiBoxBuilder.h"
#include "Framework/MultiBox/MultiBoxExtender.h"
#include "Misc/PackageName.h"
#include "Modules/ModuleManager.h"
#include "Runtime/Launch/Resources/Version.h"
#include "Textures/SlateIcon.h"

#define LOCTEXT_NAMESPACE "ReliefMappingEditor"

DECLARE_LOG_CATEGORY_EXTERN(LogReliefMappingEditorCBExtensions, Log, All);
DEFINE_LOG_CATEGORY(LogReliefMappingEditorCBExtensions);

//////////////////////////////////////////////////////////////////////////

static FContentBrowserMenuExtender_SelectedAssets ContentBrowserExtenderDelegate;
static FDelegateHandle ContentBrowserExtenderDelegateHandle;

//////////////////////////////////////////////////////////////////////////
// FContentBrowserSelectedAssetExtensionBase

struct FContentBrowserSelectedAssetExtensionBase
{
public:
	TArray<struct FAssetData> SelectedAssets;

public:
	virtual void Execute() {}
	virtual ~FContentBrowserSelectedAssetExtensionBase() {}
};

//////////////////////////////////////////////////////////////////////////
// FGenerateReliefTexture

#include "AssetToolsModule.h"
#include "IAssetTools.h"

struct FGenerateReliefTextureExtension : public FContentBrowserSelectedAssetExtensionBase
{
	EReliefMapping_Type Type;
	bool bAutoSave;
	
	
	FGenerateReliefTextureExtension()
		: Type(RelaxedConeStepMap)
		, bAutoSave(false) 
	{}
	
	void GenerateReliefTextureFromAssets(const TArray<UTexture2D*>& Textures)
	{
		if(!GEditor)
			return;
		
		const FAssetToolsModule& AssetToolsModule = FModuleManager::Get().LoadModuleChecked<FAssetToolsModule>("AssetTools");
		const FContentBrowserModule& ContentBrowserModule = FModuleManager::LoadModuleChecked<FContentBrowserModule>("ContentBrowser");
		UEditorUtilityWidgetBlueprint* ReliefMapGenerator = Cast<UEditorUtilityWidgetBlueprint>(ReliefMappingPaths::ReliefMapGenerator.TryLoad());
		UEditorUtilitySubsystem* EditorUtilitySubsystem = GEditor->GetEditorSubsystem<UEditorUtilitySubsystem>();
		
		const bool bBatchProcess = Textures.Num() > 1;
		
		for (auto TextureIt = Textures.CreateConstIterator(); TextureIt; ++TextureIt)
		{
			UTexture2D* Texture = *TextureIt;
			UEditorUtilityWidget* Widget = EditorUtilitySubsystem->SpawnAndRegisterTab(ReliefMapGenerator);
			if(Widget->Implements<UReliefMappingGeneratorInterface>())
			{
				IReliefMappingGeneratorInterface::Execute_PopulateSettings(Widget, Type, Texture, bAutoSave);
			}
		}
	}

	virtual void Execute() override
	{
		// Create sprites for any selected textures
		TArray<UTexture2D*> Textures;
		for (auto AssetIt = SelectedAssets.CreateConstIterator(); AssetIt; ++AssetIt)
		{
			const FAssetData& AssetData = *AssetIt;
			if (UTexture2D* Texture = Cast<UTexture2D>(AssetData.GetAsset()))
			{
				Textures.Add(Texture);
			}
		}

		GenerateReliefTextureFromAssets(Textures);
	}
};

//////////////////////////////////////////////////////////////////////////
// FReliefMappingEditorContentBrowserExtensions_Impl

class FReliefMappingEditorContentBrowserExtensions_Impl
{
public:
	static void ExecuteSelectedContentFunctor(TSharedPtr<FContentBrowserSelectedAssetExtensionBase> SelectedAssetFunctor)
	{
		SelectedAssetFunctor->Execute();
	}

	static void CreateReliefMapActionsSubMenu(FMenuBuilder& MenuBuilder, TArray<FAssetData> SelectedAssets)
	{
		MenuBuilder.AddSubMenu(
			LOCTEXT("ReliefMappingSubmenu", "Relief Map Actions"),
			LOCTEXT("ReliefMappingSubmenuTooltip", "Relief Map Related Actions for this texture. RCSM, POM, etcetera."),
			FNewMenuDelegate::CreateStatic(&FReliefMappingEditorContentBrowserExtensions_Impl::PopulateReliefMappingActionsMenu, SelectedAssets),
			false,
			FSlateIcon(FAppStyle::Get().GetStyleSetName(), "LandscapeEditor.NoiseTool")
		);
	}

	static void PopulateReliefMappingActionsMenu(FMenuBuilder& MenuBuilder, TArray<FAssetData> SelectedAssets)
	{
		const FName ReliefMappingEditorStyleSetName = FAppStyle::Get().GetStyleSetName();
		
		// Create RCSM
		TSharedPtr<FGenerateReliefTextureExtension> GenerateReliefTextureExtension = MakeShareable(new FGenerateReliefTextureExtension());
		GenerateReliefTextureExtension->Type = RelaxedConeStepMap;
		GenerateReliefTextureExtension->bAutoSave = true;
		GenerateReliefTextureExtension->SelectedAssets = SelectedAssets;

		const FUIAction Action_GenerateRelaxedConeStepMap(
			FExecuteAction::CreateStatic(&FReliefMappingEditorContentBrowserExtensions_Impl::ExecuteSelectedContentFunctor, StaticCastSharedPtr<FContentBrowserSelectedAssetExtensionBase>(GenerateReliefTextureExtension)));
		
		MenuBuilder.AddMenuEntry(
			LOCTEXT("CB_Extension_Texture_GenerateRCSM", "Generate Relaxed Cone Step Map (RCSM)"),
			LOCTEXT("CB_Extension_Texture_GenerateRCSM_Tooltip", "Precompute a Relaxed Cone Step Map from selected textures for efficient relief mapping."),
			FSlateIcon(ReliefMappingEditorStyleSetName, "LandscapeEditor.NoiseTool"),
			Action_GenerateRelaxedConeStepMap,
			NAME_None,
			EUserInterfaceActionType::Button);

		// Create Height
		TSharedPtr<FGenerateReliefTextureExtension> GenerateHeightTextureExtension = MakeShareable(new FGenerateReliefTextureExtension());
		GenerateHeightTextureExtension->Type = HeightMap;
		GenerateHeightTextureExtension->bAutoSave = true;
		GenerateHeightTextureExtension->SelectedAssets = SelectedAssets;

		const FUIAction Action_GenerateHeight(
			FExecuteAction::CreateStatic(&FReliefMappingEditorContentBrowserExtensions_Impl::ExecuteSelectedContentFunctor, StaticCastSharedPtr<FContentBrowserSelectedAssetExtensionBase>(GenerateHeightTextureExtension)));
		
		MenuBuilder.AddMenuEntry(
			LOCTEXT("CB_Extension_Texture_GenerateHeight", "Generate Height Relief Map (POM)"),
			LOCTEXT("CB_Extension_Texture_GenerateHeight_Tooltip", "Export a single channel heightmap from the given texture, allowing for adjustments."),
			FSlateIcon(ReliefMappingEditorStyleSetName, "LandscapeEditor.NoiseTool"),
			Action_GenerateHeight,
			NAME_None,
			EUserInterfaceActionType::Button);

		// Create Height
		TSharedPtr<FGenerateReliefTextureExtension> OpenReliefGeneratorExtension = MakeShareable(new FGenerateReliefTextureExtension());
		OpenReliefGeneratorExtension->Type = HeightMap;
		OpenReliefGeneratorExtension->bAutoSave = false;
		OpenReliefGeneratorExtension->SelectedAssets = SelectedAssets;

		const FUIAction Action_OpenWidget(
			FExecuteAction::CreateStatic(&FReliefMappingEditorContentBrowserExtensions_Impl::ExecuteSelectedContentFunctor, StaticCastSharedPtr<FContentBrowserSelectedAssetExtensionBase>(OpenReliefGeneratorExtension)));
		
		MenuBuilder.AddMenuEntry(
			LOCTEXT("CB_Extension_Texture_OpenWidget", "Open the Relief Map Generator Widget"),
			LOCTEXT("CB_Extension_Texture_OpenWidget_Tooltip", "Opens the relief map generator widget with the selected texture as the heightmap, but without auto-save enabled."),
			FSlateIcon(ReliefMappingEditorStyleSetName, "LandscapeEditor.NoiseTool"),
			Action_OpenWidget,
			NAME_None,
			EUserInterfaceActionType::Button);
		
	}

	static TSharedRef<FExtender> OnExtendContentBrowserAssetSelectionMenu(const TArray<FAssetData>& SelectedAssets)
	{
		TSharedRef<FExtender> Extender(new FExtender());

		// Temp, only support one at a time right now.
		if(SelectedAssets.Num()>1)
		{
			return Extender;
		}
		
		// Run thru the assets to determine if any meet our criteria
		bool bAnyTextures = false;
#if ENGINE_MAJOR_VERSION >= 5 && ENGINE_MINOR_VERSION >= 1
		const auto Texture2DClassPathName = UTexture2D::StaticClass()->GetClassPathName();
#endif
		for (auto AssetIt = SelectedAssets.CreateConstIterator(); AssetIt; ++AssetIt)
		{
			const FAssetData& Asset = *AssetIt;
#if ENGINE_MAJOR_VERSION >= 5 && ENGINE_MINOR_VERSION >= 1
			if(Asset.AssetClassPath == Texture2DClassPathName)
#else
			if(Asset.AssetClass == UTexture2D::StaticClass()->GetFName())
#endif
			{
				bAnyTextures = true;
				break;
			}
		}

		if (bAnyTextures)
		{
			// Add the sprite actions sub-menu extender
			Extender->AddMenuExtension(
				"GetAssetActions",
				EExtensionHook::After,
				nullptr,
				FMenuExtensionDelegate::CreateStatic(&FReliefMappingEditorContentBrowserExtensions_Impl::CreateReliefMapActionsSubMenu, SelectedAssets));
		}

		return Extender;
	}

	static TArray<FContentBrowserMenuExtender_SelectedAssets>& GetExtenderDelegates()
	{
		FContentBrowserModule& ContentBrowserModule = FModuleManager::LoadModuleChecked<FContentBrowserModule>(TEXT("ContentBrowser"));
		return ContentBrowserModule.GetAllAssetViewContextMenuExtenders();
	}
};

//////////////////////////////////////////////////////////////////////////
// FReliefMappingEditorContentBrowserExtensions

void FReliefMappingEditorContentBrowserExtensions::InstallHooks()
{
	ContentBrowserExtenderDelegate = FContentBrowserMenuExtender_SelectedAssets::CreateStatic(&FReliefMappingEditorContentBrowserExtensions_Impl::OnExtendContentBrowserAssetSelectionMenu);

	TArray<FContentBrowserMenuExtender_SelectedAssets>& CBMenuExtenderDelegates = FReliefMappingEditorContentBrowserExtensions_Impl::GetExtenderDelegates();
	CBMenuExtenderDelegates.Add(ContentBrowserExtenderDelegate);
	ContentBrowserExtenderDelegateHandle = CBMenuExtenderDelegates.Last().GetHandle();
}

void FReliefMappingEditorContentBrowserExtensions::RemoveHooks()
{
	TArray<FContentBrowserMenuExtender_SelectedAssets>& CBMenuExtenderDelegates = FReliefMappingEditorContentBrowserExtensions_Impl::GetExtenderDelegates();
	CBMenuExtenderDelegates.RemoveAll([](const FContentBrowserMenuExtender_SelectedAssets& Delegate){ return Delegate.GetHandle() == ContentBrowserExtenderDelegateHandle; });
}

//////////////////////////////////////////////////////////////////////////

#undef LOCTEXT_NAMESPACE
