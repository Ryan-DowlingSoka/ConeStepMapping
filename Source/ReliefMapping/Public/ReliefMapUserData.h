// Copyright (c) 2022 Ryan DowlingSoka

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "ReliefMapUserData.generated.h"

UENUM(BlueprintType)
enum EReliefMapping_Type
{
	/*
	* Single Channel Relaxed Cone Step Map as described by GPU Gems 3 || https://developer.nvidia.com/gpugems/gpugems3/part-iii-rendering/chapter-18-relaxed-cone-stepping-relief-mapping
	*
	* Relaxed Conestep Maps are cheaper on the GPU than Parallax Occlusion Mapping, but more expensive than Anisotropic Conestep Maps.
	* Generation time grows exponentially with output image resolution.
	* Outputs two texture channels, RCSM in (R), Height in (B)
	*/
	RelaxedConeStepMap,

	/*
	 * Two Channel Anisotropic Cone Step Map as described by Yu-Jen Chen and Yung-Yu Chuang 2009 || https://www.csie.ntu.edu.tw/~cyy/publications/papers/apsipa2009.pdf
	 * 
	 * Anisotropic Conestep Maps are cheaper on the GPU than both Parallax Occlusion Mapping and Relaxed Conestep Mapping.
	 * Generation time grows exponentially with output image resolution.
	 * Outputs three texture channels, ACSM.X in (R), ACSM.Y in (G), Height in (B)
	 *
	 * --Not Yet Implemented--
	 */
	AnisotropicConeStepMap UMETA(Hidden),

	/*
	 * One Channel Height Map
	 * 
	 * Singular height map channel used for tracing with Parallax Occlusion Mapping.
	 * Cheap on memory, but the worst cost to quality ratio.
	 * Only takes into affect heightmap adjustments, no other pre-computations.
	 * Generation time is almost instant, and grows linearly with output resolution.
	 * Outputs a single channel R8 Texture.
	 */
	HeightMap UMETA(DisplayName="Height Map (Parallax Occlusion Mapping)"),
};

UENUM(BlueprintType)
enum EReliefMapping_CommonSizes
{
	RMC_Custom		= 0	UMETA(DisplayName="Custom"),
	RMC_32x32		= 5	UMETA(DisplayName="32x32"),
	RMC_64x64		= 6	UMETA(DisplayName="64x64"),
	RMC_128x128		= 7	UMETA(DisplayName="128x128"),
	RMC_256x256		= 8	UMETA(DisplayName="256x256"),
	RMC_512x512		= 9	UMETA(DisplayName="512x512"),
	RMC_1024x1024	= 10	UMETA(DisplayName="1024x1024"),
	RMC_2048x2048	= 11	UMETA(DisplayName="2048x2048"),
	RMC_4096x4096	= 12	UMETA(DisplayName="4096x4096"),
};

UENUM(BlueprintType)
enum EReliefMapping_LinearColorChannels
{
	R,
	G,
	B,
	A,
};

/**
 * Userdata to store ReliefMap generation user data.
 */
UCLASS(BlueprintType)
class RELIEFMAPPING_API UReliefMapUserData : public UAssetUserData
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ReliefMapping")
	TEnumAsByte<EReliefMapping_Type> ReliefMapType = EReliefMapping_Type::RelaxedConeStepMap;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ReliefMapping")
	TSoftObjectPtr<UTexture2D> HeightMap = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ReliefMapping")
	TEnumAsByte<EReliefMapping_LinearColorChannels> HeightMapChannel = EReliefMapping_LinearColorChannels::R;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ReliefMapping")
	TEnumAsByte<EReliefMapping_CommonSizes> CommonResolutions = EReliefMapping_CommonSizes::RMC_256x256;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ReliefMapping")
	FIntPoint Resolution = FIntPoint(256,256);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ReliefMapping")
	int IterationBucketSize = 1;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ReliefMapping|Height Field Adjustments")
	float BlackPoint = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ReliefMapping|Height Field Adjustments")
	float WhitePoint = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ReliefMapping|Height Field Adjustments")
	float Floor = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ReliefMapping|Height Field Adjustments")
	float Ceiling = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ReliefMapping|Height Field Adjustments")
	float Bias = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ReliefMapping|Output Info")
	float MinHeight = 0;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
};
