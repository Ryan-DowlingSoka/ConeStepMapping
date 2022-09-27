// Fill out your copyright notice in the Description page of Project Settings.


#include "ReliefMapUserData.h"

void UReliefMapUserData::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	GetOuter()->Modify(true);
}
