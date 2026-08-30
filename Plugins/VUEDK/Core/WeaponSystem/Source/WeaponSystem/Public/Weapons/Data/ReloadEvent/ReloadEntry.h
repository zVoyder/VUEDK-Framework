// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ReloadEntry.generated.h"

class UAmmoTypeData;

USTRUCT(BlueprintType)
struct WEAPONSYSTEM_API FReloadEntry
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadWrite)
	int32 BehaviourIndex;
	UPROPERTY(BlueprintReadWrite)
	UAmmoTypeData* AmmoType;
	UPROPERTY(BlueprintReadWrite)
	int32 AmmoInput;

	FReloadEntry() : BehaviourIndex(0),
	                 AmmoType(nullptr),
	                 AmmoInput(0)
	{
	}
};
