// Copyright (c) 2022 Ryan DowlingSoka

#include "ReliefMappingEditorBPLibrary.h"

#include "EditorAssetLibrary.h"
#include "ReliefMapUserData.h"

#include "ReliefMappingEditor.h"
#include "AssetRegistry/AssetRegistryModule.h"

UReliefMappingEditorBPLibrary::UReliefMappingEditorBPLibrary(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
}

UReliefMapUserData* UReliefMappingEditorBPLibrary::GetReliefMapUserData(UTexture2D* Texture)
{
	if(IsValid(Texture))
	{
		UReliefMapUserData* ReliefMapUserData = Texture->GetAssetUserData<UReliefMapUserData>();
		return ReliefMapUserData;
	}
	return nullptr;
}

UReliefMapUserData* UReliefMappingEditorBPLibrary::FindOrAddReliefMapUserData(UTexture2D* Texture)
{
	if(IsValid(Texture))
	{
		if(UReliefMapUserData*  ReliefMapUserData = GetReliefMapUserData(Texture))
		{
			return ReliefMapUserData;
		}
		else
		{
			ReliefMapUserData = NewObject<UReliefMapUserData>(Texture, UReliefMapUserData::StaticClass());
			Texture->AddAssetUserData(ReliefMapUserData);
		}
	}
	return nullptr;
}

bool UReliefMappingEditorBPLibrary::FindOptionalAssetData(FString AssetPath, FAssetData& AssetData)
{
	FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
	FString PackagePath;
	//FPackageName::TryConvertFilenameToLongPackageName(AssetPath, PackagePath);
	//TArray<FString> Paths;
	//Paths.Add(PackagePath);
	//AssetRegistryModule.Get().ScanFilesSynchronous(Paths);
#if ENGINE_MAJOR_VERSION >= 5 && ENGINE_MINOR_VERSION >= 1
	AssetData = AssetRegistryModule.Get().GetAssetByObjectPath(FSoftObjectPath(*AssetPath));
#else
	AssetData = AssetRegistryModule.Get().GetAssetByObjectPath(*AssetPath);
#endif
	return AssetData.IsValid();
}

void UReliefMappingEditorBPLibrary::GetMinMaxTexture2DValuesPerChannel(
	UTexture2D* Texture,
	int MipIndex,
	FLinearColor& OutMin,
	FLinearColor& OutMax)
{
	if(Texture == nullptr)
	{
		OutMin = FLinearColor(0,0,0,0);
		OutMax = FLinearColor(1,1,1,1);
		return;
	}
	FTextureSource TextureSource = Texture->Source;
	TArray64<uint8> TextureRawData;
	TextureSource.GetMipData(TextureRawData, 0);

	const int32 BytesPerPixel = TextureSource.GetBytesPerPixel();
	const ETextureSourceFormat SourceFormat = TextureSource.GetFormat();

	const int32 Width = TextureSource.GetSizeX();
	const int32 Height = TextureSource.GetSizeY();

	OutMin = FLinearColor(1,1,1,1);
	OutMax = FLinearColor(0,0,0,0);

	FColor Color(0, 0, 0, 0);
	for (int Y = 0; Y < Height; ++Y)
	{
		for (int X = 0; X < Width; ++X)
		{
			const int32 PixelByteOffset = (X + Y * Width) * BytesPerPixel;
			const uint8* PixelPtr = TextureRawData.GetData() + PixelByteOffset;

			switch (SourceFormat)
			{
			case TSF_BGRA8:
			case TSF_BGRE8:
				{
					Color = *((FColor*)PixelPtr);
					break;
				}
			case TSF_G8:
				{
					const uint8 Intensity = *PixelPtr;
					Color = FColor(Intensity, Intensity, Intensity, Intensity);
					break;
				}
			default:
				{
					ensureMsgf(false, TEXT("Unknown Format"));
					break;
				}
			}

			FLinearColor LinearColor = Color;
			for( int i = 0; i < 4; i++ )
			{
				const float C = LinearColor.Component(i);
				float& MinRef = OutMin.Component(i);
				float& MaxRef = OutMax.Component(i);

				MinRef = FMath::Min(MinRef, C);
				MaxRef = FMath::Max(MaxRef, C);
			}
		}
	}
}

void UReliefMappingEditorBPLibrary::SaveLoadedAssetSilently(UObject* AssetToSave, bool bOnlyIfDirty)
{
	FSilentOperationContext SilentOperationContext;
	BeginSilentOperation(SilentOperationContext);
	UEditorAssetLibrary::SaveLoadedAsset(AssetToSave, bOnlyIfDirty);
	EndSilentOperation(SilentOperationContext);
}

void UReliefMappingEditorBPLibrary::BeginSilentOperation(FSilentOperationContext& SilentOperationContext)
{
	SilentOperationContext = FSilentOperationContext();
	GIsSilent = true;
}

void UReliefMappingEditorBPLibrary::EndSilentOperation(FSilentOperationContext& SilentOperationContext)
{
	GIsSilent = SilentOperationContext.CachedGIsSilent;
}



