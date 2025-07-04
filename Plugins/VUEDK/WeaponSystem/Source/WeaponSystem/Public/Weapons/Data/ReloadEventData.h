// Copyright zVoyder, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "ReloadEventData.generated.h"

USTRUCT(BlueprintType)
struct WEAPONSYSTEM_API FReloadEventData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	int32 AmmoToReload;

	FReloadEventData(): AmmoToReload(0)
	{
	}

	explicit FReloadEventData(const int32 InAmmo): AmmoToReload(InAmmo)
	{
	}
};
