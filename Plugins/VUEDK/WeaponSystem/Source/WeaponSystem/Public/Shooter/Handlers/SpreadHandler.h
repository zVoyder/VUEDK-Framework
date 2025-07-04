// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ShooterHandlerBase.h"
#include "UObject/Object.h"
#include "SpreadHandler.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(
	FOnStartedChangeSpread,
	float, DefaultSpread,
	float, Spread
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
	FOnProcessingSpread,
	float, Spread
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(
	FOnCompletedChangeSpread,
	float, DefaultSpread,
	float, Spread
);

constexpr int32 DynamicIdleState = 0;
constexpr int32 DynamicAddState = 1;
constexpr int32 DynamicRecoverState = 2;

UCLASS(BlueprintType)
class WEAPONSYSTEM_API USpreadHandler : public UShooterHandlerBase
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable, Category = Events)
	FOnStartedChangeSpread OnStartedChangeSpread;
	UPROPERTY(BlueprintAssignable, Category = Events)
	FOnProcessingSpread OnProcessingSpread;
	UPROPERTY(BlueprintAssignable, Category = Events)
	FOnCompletedChangeSpread OnCompletedChangeSpread;

private:
	// -- Spread --
	float BaseSpread;
	float TargetSpread;
	float SpreadChangeRate;
	float SpreadDir;
	bool bOverrideDefaultSpread = false;
	bool bIsProcessingSpread = false;

	// -- Dynamic Spread --
	float DynamicSpread;
	float TargetDynamicSpread;
	float DynamicSpreadChangeRate;
	float DynamicSpreadRecoveryRate;
	float DynamicSpreadDir;
	int32 CurrentDynamicSpreadState;

public:
	virtual void Tick(float DeltaTime) override;

	/**
	 * Adds dynamic spread to the handler, increasing spread over time.
	 * @param AddSpread - The amount of spread to add.
	 * @param ChangeRate - The rate at which spread is added.
	 * @param RecoveryRate - The rate at which spread recovers.
	 */
	void AddDynamicSpread(float AddSpread, const float ChangeRate = 1.0f, const float RecoveryRate = 1.0f);

	/**
	 * Adds dynamic spread using a curve for more complex spread behavior.
	 */
	void AddDynamicSpreadWithCurve();

	/**
	 * Instantly sets the spread to a specific value.
	 * @param InSpread - The spread value to set.
	 * @param bOverrideDefault - Whether to override the default spread value.
	 */
	void InstantSetSpread(const float InSpread, const bool bOverrideDefault = false);

	/**
	 * Sets the spread to a specific value, optionally overriding the default.
	 * @param InSpread - The spread value to set.
	 * @param ChangeRate - The rate at which to change to the new spread.
	 * @param bOverrideDefault - Whether to override the default spread value.
	 */
	void SetSpread(float InSpread, const float ChangeRate = 1.0f, const bool bOverrideDefault = false);

	/**
	 * Resets the spread to its default value.
	 * @param ChangeRate - The rate at which to reset the spread.
	 */
	void ResetSpread(const float ChangeRate = 1.0f);

	/**
	 * Gets the current spread value.
	 * @return The current spread as a float.
	 */
	float GetSpread() const;

	/**
	 * Checks if the spread handler is currently processing a spread change.
	 * @return true if processing, false otherwise.
	 */
	bool IsProcessingSpread() const;

protected:
	/**
	 * Called when the handler is initialized.
	 */
	virtual void OnInit() override;

private:
	/**
	 * Processes the spread logic each frame.
	 * @param DeltaTime - The time elapsed since the last tick.
	 */
	void ProcessSpread(const float DeltaTime);

	/**
	 * Processes the logic for adding dynamic spread each frame.
	 * @param DeltaTime - The time elapsed since the last tick.
	 */
	void ProcessAddDynamicSpread(float DeltaTime);

	/**
	 * Processes the logic for recovering from dynamic spread each frame.
	 * @param DeltaTime - The time elapsed since the last tick.
	 */
	void ProcessRecoverDynamicSpread(float DeltaTime);

	/**
	 * Ends the current spread change process.
	 */
	void EndSpreadChange();

	/**
	 * Ends the dynamic add spread change process.
	 */
	void EndDynamicAddSpreadChange();

	/**
	 * Ends the dynamic recover spread change process.
	 */
	void EndDynamicRecoverSpreadChange();

	/**
	 * Transitions the handler to the idle state for dynamic spread.
	 */
	void TransitionToDynamicIdleState();

	/**
	 * Transitions the handler to the add state for dynamic spread.
	 */
	void TransitionToDynamicAddState();

	/**
	 * Transitions the handler to the recover state for dynamic spread.
	 */
	void TransitionToDynamicRecoverState();
};
