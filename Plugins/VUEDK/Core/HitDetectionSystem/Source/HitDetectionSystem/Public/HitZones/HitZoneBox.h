// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "HitZoneHandler.h"
#include "Components/BoxComponent.h"
#include "Interfaces/HitZone.h"
#include "HitZoneBox.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class HITDETECTIONSYSTEM_API UHitZoneBox : public UBoxComponent, public IHitZone
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Instanced)
	UHitZoneHandler* HitZoneHandler;

public:
	UHitZoneBox();

	virtual UHitZoneHandler* GetHitZoneHandler_Implementation() const override;

protected:
	virtual void BeginPlay() override;
};
