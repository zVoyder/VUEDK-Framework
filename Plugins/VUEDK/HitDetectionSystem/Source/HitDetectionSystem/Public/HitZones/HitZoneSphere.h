// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "HitZoneHandler.h"
#include "Components/SphereComponent.h"
#include "Interfaces/HitZone.h"
#include "HitZoneSphere.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class HITDETECTIONSYSTEM_API UHitZoneSphere : public USphereComponent, public IHitZone
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Instanced)
	UHitZoneHandler* HitZoneHandler;

public:
	UHitZoneSphere();

	UHitZoneHandler* GetHitZoneHandler_Implementation() const override;

protected:
	virtual void BeginPlay() override;
};
