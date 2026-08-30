// Copyright VUEDK, Inc. All Rights Reserved.

#include "DismemberLimbOperation.h"
#include "DismembererSystem.h"

void UDismemberLimbOperation::Init(UDismemberer* InDismemberer)
{
	Dismemberer = InDismemberer;
}

ADismemberedLimb* UDismemberLimbOperation::SpawnLimbActor_Implementation(TSubclassOf<ADismemberedLimb> DismemberedLimbClass)
{
	UWorld* World = GetWorld();
	if (!IsValid(World))
	{
		UE_LOG(LogDismembermentSystem, Error, TEXT("UDismemberLimbOperation::SpawnLimbActor: Dismemberer has not a valid World."));
		return nullptr;
	}
	
	if (!IsValid(DismemberedLimbClass))
	{
		UE_LOG(LogDismembermentSystem, Error, TEXT("UDismemberLimbOperation::SpawnLimbActor: DismemberedLimbClass is not valid."));
		return nullptr;
	}
	
	return World->SpawnActor<ADismemberedLimb>(DismemberedLimbClass);
}

UWorld* UDismemberLimbOperation::GetWorld() const
{
	return Dismemberer->GetWorld();
}

#if WITH_EDITOR
bool UDismemberLimbOperation::ImplementsGetWorld() const
{
	return true;
}
#endif
