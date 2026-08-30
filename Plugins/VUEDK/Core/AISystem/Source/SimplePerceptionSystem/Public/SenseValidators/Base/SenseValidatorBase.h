// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Base/SimplePerceiverObjectBase.h"
#include "SenseValidatorBase.generated.h"

UCLASS(Abstract, Blueprintable, BlueprintType, EditInlineNew, DefaultToInstanced)
class SIMPLEPERCEPTIONSYSTEM_API USenseValidatorBase : public USimplePerceiverObjectBase
{
	GENERATED_BODY()

public:
	bool Validate(AActor* Actor) const;

protected:
	UFUNCTION(BlueprintNativeEvent, meta = (DisplayName = "Validate"))
	bool ReceiveValidate(AActor* Actor) const;
};
