// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Shooter/Behaviours/ShooterBehaviourBase.h"
#include "HandlersFactory.generated.h"

UCLASS()
class WEAPONSYSTEM_API UHandlersFactory : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	template<typename T>
	static T* CreateHandler(UShooterBehaviourBase* InBehaviour);
};
