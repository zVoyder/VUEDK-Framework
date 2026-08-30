// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "AmmoTypeData.generated.h"

UCLASS(NotBlueprintable, BlueprintType)
class WEAPONSYSTEM_API UAmmoTypeData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, AdvancedDisplay)
	FGuid AmmoID;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText AmmoName;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText AmmoDescription;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FSlateBrush AmmoIcon;

	UAmmoTypeData(): AmmoID(FGuid::NewGuid())
	{
	}
};
