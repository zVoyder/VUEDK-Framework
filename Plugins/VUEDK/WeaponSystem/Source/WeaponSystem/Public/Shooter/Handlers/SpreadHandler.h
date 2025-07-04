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

	void AddDynamicSpread(float AddSpread, const float ChangeRate = 1.0f, const float RecoveryRate = 1.0f);

	void AddDynamicSpreadWithCurve();

	void InstantSetSpread(const float InSpread, const bool bOverrideDefault = false);

	void SetSpread(float InSpread, const float ChangeRate = 1.0f, const bool bOverrideDefault = false);

	void ResetSpread(const float ChangeRate = 1.0f);

	float GetSpread() const;

	bool IsProcessingSpread() const;

protected:
	virtual void OnInit() override;

private:
	void ProcessSpread(const float DeltaTime);

	void ProcessAddDynamicSpread(float DeltaTime);

	void ProcessRecoverDynamicSpread(float DeltaTime);

	void EndSpreadChange();

	void EndDynamicAddSpreadChange();

	void EndDynamicRecoverSpreadChange();

	void TransitionToDynamicIdleState();

	void TransitionToDynamicAddState();

	void TransitionToDynamicRecoverState();
};
