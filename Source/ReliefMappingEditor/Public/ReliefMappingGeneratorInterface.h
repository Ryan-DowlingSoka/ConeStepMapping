// Copyright (c) 2022 Ryan DowlingSoka

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ReliefMapUserData.h"
#include "ReliefMappingGeneratorInterface.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UReliefMappingGeneratorInterface : public UInterface
{
	GENERATED_UINTERFACE_BODY()
};

/**
 * Interface to allow control of the Relief Map Generator Widget from C++.
 */
class RELIEFMAPPINGEDITOR_API IReliefMappingGeneratorInterface : public IInterface
{
	GENERATED_IINTERFACE_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Relief Mapping")
	void PopulateSettings(EReliefMapping_Type Type, UTexture2D* Heightmap, bool bAutoSave);
};
