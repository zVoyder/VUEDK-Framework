// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ReloadEntry.h"
#include "ReloadMode.h"
#include "Engine/DataAsset.h"
#include "ReloadRequest.generated.h"

class UAmmoTypeData;

USTRUCT(BlueprintType)
struct WEAPONSYSTEM_API FReloadRequest
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	TArray<FReloadEntry> Entries;
	UPROPERTY(BlueprintReadWrite)
	EReloadMode ReloadMode;

	FReloadRequest() : Entries(),
	                   ReloadMode(EReloadMode::Parallel)
	{
	}
};
