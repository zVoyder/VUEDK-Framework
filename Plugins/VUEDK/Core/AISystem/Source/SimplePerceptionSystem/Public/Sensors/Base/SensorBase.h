// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Base/SimplePerceiverObjectBase.h"
#include "UObject/Object.h"
#include "SensorBase.generated.h"

class USenseBase;
struct FPerceptionContext;

UCLASS(Abstract, Blueprintable, BlueprintType, EditInlineNew, DefaultToInstanced)
class SIMPLEPERCEPTIONSYSTEM_API USensorBase : public USimplePerceiverObjectBase
{
	GENERATED_BODY()

private:
	UPROPERTY()
	USenseBase* Sense;
	
public:
	void SetupSense(USenseBase* InSense);
	
	void Process(const float DeltaTime);

protected:
	UFUNCTION(BlueprintNativeEvent, meta = (DisplayName = "Process"))
	void ReceiveProcess(const float DeltaTime);

	UFUNCTION(BlueprintCallable)
	void SenseActor(AActor* Actor) const;
};
