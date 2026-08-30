// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Base/SenseValidatorBase.h"
#include "SenseValidator_LineOfSight.generated.h"

UCLASS()
class SIMPLEPERCEPTIONSYSTEM_API USenseValidator_LineOfSight : public USenseValidatorBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sense Validator")
	TEnumAsByte<ETraceTypeQuery> TraceChannel = UEngineTypes::ConvertToTraceType(ECC_Visibility);
	
protected:
	virtual bool ReceiveValidate_Implementation(AActor* Actor) const override;
};
