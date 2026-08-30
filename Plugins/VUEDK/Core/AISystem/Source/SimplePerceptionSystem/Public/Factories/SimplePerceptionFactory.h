// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Senses/Base/SenseBase.h"
#include "SimplePerceptionFactory.generated.h"

UCLASS()
class SIMPLEPERCEPTIONSYSTEM_API USimplePerceptionFactory : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	static USenseBase* CreateSense(const USenseBase* SenseTemplate, USimplePerceiver* Perceiver);
};
