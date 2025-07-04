// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "StatOperation.h"
#include "Data/CoreStatData.h"
#include "Data/SpecialStatData.h"
#include "BridgeStatOperation.generated.h"

class UStatsBridgeBase;

UCLASS()
class RPGSYSTEM_API UBridgeStatOperation : public UStatOperation
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly)
	UCoreStatData* CoreStatData;

public:
	/**
	 * Initializes the bridge stat operation with the given payload, modifier value, and core stat data.
	 * @param InPayload - The payload object for initialization.
	 * @param InModifierValue - The modifier value to apply.
	 * @param InCoreStatData - The core stat data to associate with this operation.
	 */
	void Init(UObject* InPayload, const float InModifierValue, UCoreStatData* InCoreStatData);

	/**
	 * Gets the stats bridge associated with this operation.
	 * @return Pointer to the UStatsBridgeBase instance, or nullptr if not found.
	 */
	UFUNCTION(BlueprintPure)
	UStatsBridgeBase* GetStatsBridge() const;

	/**
	 * Gets the default value of the associated core stat.
	 * @return The default value of the core stat as a float.
	 */
	UFUNCTION(BlueprintPure)
	float GetCoreStatDefaultValue() const;

	/**
	 * Gets the value of a specific special stat associated with this operation.
	 * @param SpecialStat - The special stat data to query.
	 * @return The value of the special stat as a float.
	 */
	UFUNCTION(BlueprintPure)
	float GetValueOfSpecialStat(const USpecialStatData* SpecialStat) const;
};
