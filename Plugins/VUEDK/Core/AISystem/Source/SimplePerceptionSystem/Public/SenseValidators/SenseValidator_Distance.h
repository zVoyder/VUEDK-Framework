// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Base/SenseValidatorBase.h"
#include "SenseValidator_Distance.generated.h"

UCLASS()
class SIMPLEPERCEPTIONSYSTEM_API USenseValidator_Distance : public USenseValidatorBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sense Validator")
	float MaxDistance = 2000.0f;

protected:
	virtual bool ReceiveValidate_Implementation(AActor* Actor) const override;
};
