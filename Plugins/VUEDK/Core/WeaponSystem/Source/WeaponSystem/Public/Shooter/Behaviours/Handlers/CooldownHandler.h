// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Base/ShooterHandlerBase.h"
#include "UObject/Object.h"
#include "CooldownHandler.generated.h"

UCLASS(BlueprintType)
class WEAPONSYSTEM_API UCooldownHandler : public UShooterHandlerBase
{
	GENERATED_BODY()

private:
	bool bIsInCooldown;
	float CooldownRemaining;

public:
	virtual void TickHandler(const float DeltaTime) override;
	
	/**
	 * Resets the cooldown, making the handler ready for the next action.
	 */
	void ResetCooldown();

	/**
	 * Starts the cooldown process.
	 */
	void StartCooldown();

	/**
	 * Checks if the handler is currently in cooldown.
	 * @return true if in cooldown, false otherwise.
	 */
	bool IsInCooldown() const;

private:
	/**
	 * Processes the cooldown logic each frame.
	 * @param DeltaTime The time elapsed since the last tick.
	 */
	void ProcessCooldown(float DeltaTime);

	/**
	 * Ends the shoot cooldown and resets the handler state.
	 */
	void EndShootCooldown();
};
