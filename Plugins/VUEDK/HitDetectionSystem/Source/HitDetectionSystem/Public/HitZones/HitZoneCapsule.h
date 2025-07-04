// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "HitZoneHandler.h"
#include "Components/CapsuleComponent.h"
#include "Interfaces/HitZone.h"
#include "HitZoneCapsule.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class HITDETECTIONSYSTEM_API UHitZoneCapsule : public UCapsuleComponent, public IHitZone
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Instanced)
	UHitZoneHandler* HitZoneHandler;

public:
	UHitZoneCapsule();

	UHitZoneHandler* GetHitZoneHandler_Implementation() const override;

protected:
	virtual void BeginPlay() override;
};
