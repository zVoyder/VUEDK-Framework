// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Weapons/Data/AmmoTypeData.h"
#include "MagData.generated.h"

USTRUCT(BlueprintType)
struct WEAPONSYSTEM_API FMagData
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UAmmoTypeData* AmmoType;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = 0, UIMin = 0, ToolTip = "Maximum number of ammo that the magazine can hold."))
	int32 DefaultSize;

	FMagData() : AmmoType(nullptr),
	             DefaultSize(0)
	{
	}
};
