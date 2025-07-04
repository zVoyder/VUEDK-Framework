// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "StatOperation.generated.h"

UCLASS(Blueprintable, BlueprintType)
class RPGSYSTEM_API UStatOperation : public UObject
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, meta = (ExposeOnSpawn = "true"))
	UObject* Payload;
	UPROPERTY(BlueprintReadOnly, meta = (ExposeOnSpawn = "true"))
	float ModifierValue;
	
public:
	/**
	 * Initializes the stat operation with the given payload and modifier value.
	 * @param InPayload The payload object for initialization.
	 * @param InModifierValue The modifier value to apply.
	 */
	void Init(UObject* InPayload, const float InModifierValue);

	/**
	 * Gets the payload associated with this operation.
	 * @return Pointer to the payload object, or nullptr if not set.
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintPure)
	float GetResultOperation() const;

protected:
	UFUNCTION(BlueprintNativeEvent)
	void OnInit();
};
