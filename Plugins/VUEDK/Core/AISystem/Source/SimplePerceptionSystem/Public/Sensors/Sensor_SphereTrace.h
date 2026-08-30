// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Base/SensorBase.h"
#include "Sensor_SphereTrace.generated.h"

UCLASS()
class SIMPLEPERCEPTIONSYSTEM_API USensor_SphereTrace : public USensorBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sensors")
	float Radius = 2000.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sensors")
	TEnumAsByte<ETraceTypeQuery> TraceChannel = UEngineTypes::ConvertToTraceType(ECC_Visibility);
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,
		meta = (EditCondition = "bUseFilter", ToolTip = "If true, only actors of the specified class will be sensed. If false, all actors will be sensed."), Category = "Sensors")
	TSubclassOf<AActor> ActorClassFilter = AActor::StaticClass();

private:
	UPROPERTY()
	bool bUseFilter;

protected:
	virtual void ReceiveProcess_Implementation(const float DeltaTime) override;
};
