// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Base/SenseValidatorBase.h"
#include "SenseValidator_IsA.generated.h"

UCLASS()
class SIMPLEPERCEPTIONSYSTEM_API USenseValidator_IsA : public USenseValidatorBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Sense Validator")
	TSubclassOf<AActor> ActorClass;
	
protected:
	virtual bool ReceiveValidate_Implementation(AActor* Actor) const override;
};
