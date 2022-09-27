// MIT License
//
// Copyright (c) 2022 Ryan DowlingSoka
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "ReliefMappingEditorBPLibrary.generated.h"

USTRUCT(BlueprintType)
struct FSilentOperationContext
{
	GENERATED_USTRUCT_BODY()
	bool CachedGIsSilent;

	FSilentOperationContext()
	{
		CachedGIsSilent = GIsSilent;
	}
};

class UReliefMapUserData;
/*
 * Relief Mapping Editor Blueprint Library Function
*/
UCLASS()
class UReliefMappingEditorBPLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_UCLASS_BODY()

	/* Gets a ReliefMapUserData on the given texture asset. */
	UFUNCTION(BlueprintCallable, Category="ReliefMapping")
	static UPARAM(DisplayName="Data") UReliefMapUserData* GetReliefMapUserData(UTexture2D* Texture);
	
	/* Finds or Adds a ReliefMapUserData on the given texture asset. */
	UFUNCTION(BlueprintCallable, Category="ReliefMapping")
	static UPARAM(DisplayName="Data") UReliefMapUserData* FindOrAddReliefMapUserData(UTexture2D* Texture);
	
	/* Finds an AssetData path and returns false if the path doesn't exist. */
	UFUNCTION(BlueprintCallable, Category="ReliefMapping")
	static UPARAM(DisplayName="Asset Exists?") bool FindOptionalAssetData(FString AssetPath, FAssetData& AssetData);

	/* Gets the minimum and maximum values for a Texture2D for manipulation in blueprints.
	 * Important:
	 * This returns two LinearColors with the min and max values per channel.
	 * e.g.: if there are three pixels (0,0,1) (0,.5,0), (.1,0,0)
	 *		The min values will be (  0,  0, 0)
	 *		The max values will be ( .1, .5, 1)
	 * It is treating each channel independently.
	 *
	 * If Texture is null returns (0,0,0) for min and (1,1,1) for max.
	 */
	UFUNCTION(BlueprintCallable, Category="Texture")
	static void GetMinMaxTexture2DValuesPerChannel(UTexture2D* Texture, int MipIndex, FLinearColor& OutMin, FLinearColor& OutMax);

	UFUNCTION(BlueprintCallable, Category="EditorScripting")
	static void SaveLoadedAssetSilently(UObject* AssetToSave, bool bOnlyIfDirty);

	UFUNCTION(BlueprintCallable, Category="EditorScripting")
	static void BeginSilentOperation(FSilentOperationContext& SilentOperationContext);

	UFUNCTION(BlueprintCallable, Category="EditorScripting")
	static void EndSilentOperation(UPARAM(ref) FSilentOperationContext& SilentOperationContext);
};
