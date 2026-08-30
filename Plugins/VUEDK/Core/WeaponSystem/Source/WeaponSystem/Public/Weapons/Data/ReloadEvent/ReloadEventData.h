// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ReloadRequest.h"
#include "UObject/Object.h"
#include "ReloadEventData.generated.h"

class UAmmoTypeData;

USTRUCT(BlueprintType)
struct WEAPONSYSTEM_API FReloadEventData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	FReloadRequest Request;
	UPROPERTY(BlueprintReadOnly)
	TMap<UAmmoTypeData*, int32> ReloadAmmoByType;
};
