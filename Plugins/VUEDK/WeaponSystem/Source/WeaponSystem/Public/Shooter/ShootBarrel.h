// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ShootPoint.h"
#include "Components/SceneComponent.h"
#include "ShootBarrel.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class WEAPONSYSTEM_API UShootBarrel : public USceneComponent
{
	GENERATED_BODY()

public:
	UShootBarrel();

	/**
	 * Gets the shoot points children of this shoot barrel.
	 * @return An array of shoot points that are children of this shoot barrel.
	 */
	UFUNCTION(BlueprintPure)
	TArray<UShootPoint*> GetShootPointsChildren() const;
};
