// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Base/SensorBase.h"
#include "Sensor_Damage.generated.h"

UCLASS()
class SIMPLEPERCEPTIONSYSTEM_API USensor_Damage : public USensorBase
{
	GENERATED_BODY()

public:
	virtual void Init(USimplePerceiver* InSimplePerceiver) override;
	
	UFUNCTION()
	void OnTakeAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);
};
