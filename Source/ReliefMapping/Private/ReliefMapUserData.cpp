// Copyright (c) 2022 Ryan DowlingSoka


#include "ReliefMapUserData.h"

#if WITH_EDITOR
void UReliefMapUserData::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	GetOuter()->Modify(true);
}
#endif
