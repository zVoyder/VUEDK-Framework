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
	
	UFUNCTION(BlueprintPure)
	TArray<UShootPoint*> GetShootPointsChildren() const;
};
