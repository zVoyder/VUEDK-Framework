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
	/**
	 * Creates and returns a new recoil handler for the specified shooter behaviour and shoot data.
	 * @param InBehaviour - The shooter behaviour to associate with the handler.
	 * @param ShootData - The shoot data to use for initialization.
	 * @return Pointer to the created URecoilHandler instance.
	 */
	static URecoilHandler* CreateRecoilHandler(UShooterBehaviourBase* InBehaviour, FShootData& ShootData);

	/**
	 * Creates and returns a new cooldown handler for the specified shooter behaviour and shoot data.
	 * @param InBehaviour - The shooter behaviour to associate with the handler.
	 * @param ShootData - The shoot data to use for initialization.
	 * @return Pointer to the created UCooldownHandler instance.
	 */
	static UCooldownHandler* CreateCooldownHandler(UShooterBehaviourBase* InBehaviour, FShootData& ShootData);

	/**
	 * Creates and returns a new spread handler for the specified shooter behaviour and shoot data.
	 * @param InBehaviour - The shooter behaviour to associate with the handler.
	 * @param ShootData - The shoot data to use for initialization.
	 * @return Pointer to the created USpreadHandler instance.
	 */
	static USpreadHandler* CreateSpreadHandler(UShooterBehaviourBase* InBehaviour, FShootData& ShootData);
};
