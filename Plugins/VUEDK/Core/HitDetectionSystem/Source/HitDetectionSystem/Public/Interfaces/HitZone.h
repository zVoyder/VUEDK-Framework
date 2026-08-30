// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "HitZones/HitZoneHandler.h"
#include "UObject/Interface.h"
#include "HitZone.generated.h"

UINTERFACE()
class UHitZone : public UInterface
{
	GENERATED_BODY()
};

class HITDETECTIONSYSTEM_API IHitZone
{
	GENERATED_BODY()

public:
	/**
	 * Returns the HitZoneHandler associated with this HitZone.
	 * @return The HitZoneHandler instance.
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Hit Zone")
	UHitZoneHandler* GetHitZoneHandler() const;
};
