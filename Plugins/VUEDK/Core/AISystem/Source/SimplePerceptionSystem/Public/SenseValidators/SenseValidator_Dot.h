// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Base/SenseValidatorBase.h"
#include "SenseValidator_Dot.generated.h"

UCLASS()
class SIMPLEPERCEPTIONSYSTEM_API USenseValidator_Dot : public USenseValidatorBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sense Validator", meta = (ClampMin = "0", ClampMax = "180", UIMin = "0", UIMax = "180"))
	float DotThreshold = 45.0f;
	
protected:
	virtual bool ReceiveValidate_Implementation(AActor* Actor) const override;
};
