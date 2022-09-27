// Copyright (c) 2022 Ryan DowlingSoka


#include "ReliefMapUserData.h"

void UReliefMapUserData::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	GetOuter()->Modify(true);
}
