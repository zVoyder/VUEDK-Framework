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
	void Init(UObject* InPayload, const float InModifierValue, UCoreStatData* InCoreStatData);

	UFUNCTION(BlueprintPure)
	UStatsBridgeBase* GetStatsBridge() const;

	UFUNCTION(BlueprintPure)
	float GetCoreStatDefaultValue() const;

	UFUNCTION(BlueprintPure)
	float GetValueOfSpecialStat(const USpecialStatData* SpecialStat) const;
};
