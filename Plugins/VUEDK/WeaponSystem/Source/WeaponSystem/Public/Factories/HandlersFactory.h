// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Shooter/Handlers/CooldownHandler.h"
#include "Shooter/Handlers/RecoilHandler.h"
#include "Shooter/Handlers/SpreadHandler.h"
#include "Shooter/Data/ShootData.h"
#include "HandlersFactory.generated.h"

UCLASS()
class WEAPONSYSTEM_API UHandlersFactory : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	static URecoilHandler* CreateRecoilHandler(UShooterBehaviourBase* InBehaviour, FShootData& ShootData);

	static UCooldownHandler* CreateCooldownHandler(UShooterBehaviourBase* InBehaviour, FShootData& ShootData);

	static USpreadHandler* CreateSpreadHandler(UShooterBehaviourBase* InBehaviour, FShootData& ShootData);
};
