// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Data/ResourceAttributeData.h"
#include "UObject/Object.h"
#include "ResourceAttribute.generated.h"

class UResourceAttributesManager;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(
	FOnResourceAttributeChanged,
	float,
	OldValue,
	float,
	NewValue
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(
	FOnResourceAttributeIncreased,
	float,
	IncreasedAmount,
	float,
	OldValue,
	float,
	NewValue
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(
	FOnResourceAttributeDecreased,
	float,
	DecreasedAmount,
	float,
	OldValue,
	float,
	NewValue
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(
	FOnResourceAttributeMinValueChanged,
	float,
	OldMinValue,
	float,
	NewMinValue
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(
	FOnResourceAttributeMaxValueChanged,
	float,
	OldMaxValue,
	float,
	NewMaxValue
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
	FOnResourceAttributeReachedMaxValue,
	float,
	CurrentValue
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
	FOnResourceAttributeReachedMinValue,
	float,
	CurrentValue
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
	FOnResourceAttributeRegenerationStarted,
	float,
	CurrentValue
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
	FOnResourceAttributeRegenerationStopped,
	float,
	CurrentValue
);

UCLASS(Blueprintable)
class RESOURCEATTRIBUTESSYSTEM_API UResourceAttribute : public UObject, public FTickableGameObject
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnResourceAttributeChanged OnResourceAttributeChanged;
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnResourceAttributeIncreased OnResourceAttributeIncreased;
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnResourceAttributeDecreased OnResourceAttributeDecreased;
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnResourceAttributeMinValueChanged OnResourceAttributeMinValueChanged;
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnResourceAttributeMaxValueChanged OnResourceAttributeMaxValueChanged;
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnResourceAttributeReachedMinValue OnResourceAttributeReachedMinValue;
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnResourceAttributeReachedMaxValue OnResourceAttributeReachedMaxValue;
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnResourceAttributeRegenerationStarted OnResourceAttributeRegenStarted;
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnResourceAttributeRegenerationStopped OnResourceAttributeRegenStopped;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Resource Attribute|Data")
	UResourceAttributeData* ResourceAttributeData;

private:
	UPROPERTY()
	UResourceAttributesManager* ResourceAttributesManager = nullptr;
	bool bIsRegenerating = false;
	float MinValue = 0.0f;
	float MaxValue = 100.0f;
	float RegenerationRate = 0.1f;
	float CurrentValue = 0.0f;
	FTimerHandle RegenTimerHandle;

public:
	/**
	 * Initializes the resource attribute with the given data and manager.
	 * @param InResourceAttributeData The data asset containing attribute configuration.
	 * @param InResourceAttributesManager The manager responsible for handling this attribute.
	 */
	UFUNCTION(BlueprintCallable)
	void Init(UResourceAttributeData* InResourceAttributeData, UResourceAttributesManager* InResourceAttributesManager);
	
	virtual void Tick(float DeltaTime) override;

	virtual bool IsTickable() const override;
	
	virtual TStatId GetStatId() const override;
	
	/**
	 * Checks if the attribute is currently at its minimum value.
	 * @return True if the attribute is at its minimum value, false otherwise.
	 */
	UFUNCTION(BlueprintPure)
	bool IsAtMin() const;

	/**
	 * Checks if the attribute is currently at its maximum value.
	 * @return True if the attribute is at its maximum value, false otherwise.
	 */
	UFUNCTION(BlueprintPure)
	bool IsAtMax() const;
	
	/**
	 * Gets the current value of the attribute.
	 * @return The current attribute value.
	 */
	UFUNCTION(BlueprintPure)
	float GetValue() const;

	/**
	 * Gets the current value of the attribute as a percentage of its maximum value.
	 * @return The current value as a percentage.
	 */
	UFUNCTION(BlueprintPure)
	float GetValueAsPercent() const;

	/**
	 * Returns the current minimum value of the attribute.
	 * @return The minimum value.
	 */
	UFUNCTION(BlueprintPure)
	float GetMinValue() const;

	/**
	 * Returns the current maximum value of the attribute.
	 * @return The maximum value.
	 */
	UFUNCTION(BlueprintPure)
	float GetMaxValue() const;

	/**
	 * Returns the current regeneration rate of the attribute.
	 */
	float GetRegenerationRate() const;

	/**
	 * Sets the attribute value to the specified amount.
	 * @param NewValue The new value to set.
	 */
	UFUNCTION(BlueprintCallable)
	void SetValue(const float NewValue);

	/**
	 * Modifies the attribute value by the specified amount (positive or negative).
	 * @param Amount The amount to add or subtract from the current value.
	 */
	UFUNCTION(BlueprintCallable)
	void ModifyValue(const float Amount);
	
	/**
	 * Sets the attribute value to its minimum allowed value.
	 */
	UFUNCTION(BlueprintCallable)
	void DrainToMin();

	/**
	 * Sets the attribute value to its maximum allowed value.
	 */
	UFUNCTION(BlueprintCallable)
	void FillToMax();
	
	/**
	 * Sets the minimum allowed value for the attribute.
	 * @param NewMinValue The new minimum value.
	 */
	UFUNCTION(BlueprintCallable)
	void SetMinValue(const float NewMinValue);

	/**
	 * Sets the maximum allowed value for the attribute.
	 * @param NewMaxValue The new maximum value.
	 */
	UFUNCTION(BlueprintCallable)
	void SetMaxValue(const float NewMaxValue);

	/**
	 * Sets the regeneration rate for the attribute.
	 * @param NewRegenerationRate The new regeneration rate.
	 */
	UFUNCTION(BlueprintCallable)
	void SetRegenerationRate(const float NewRegenerationRate);

	/**
	 * Starts the regeneration process for the attribute, with an optional delay.
	 * @param Delay Time in seconds before regeneration starts (default is 0).
	 */
	UFUNCTION(BlueprintCallable)
	void StartRegen(const float Delay = 0.0f);

	/**
	 * Stops the regeneration process for the attribute.
	 */
	UFUNCTION(BlueprintCallable)
	void StopRegen();

private:
	/**
	 * Handles the regeneration logic, called every tick with DeltaTime.
	 * @param DeltaTime Time elapsed since last call.
	 */
	void ProcessRegen(const float DeltaTime);

	/**
	 * Called when the attribute value increases.
	 * @param OldValue The value before the increase.
	 */
	void OnAttributeIncreased(const float OldValue);

	/**
	 * Called when the attribute value decreases.
	 * @param OldValue The value before the decrease.
	 */
	void OnAttributeDecreased(const float OldValue);

	/**
	 * Called when regeneration starts.
	 */
	UFUNCTION()
	void OnRegenStarted();

	/**
	 * Checks the validity or state of the attribute (implementation-specific).
	 */
	bool Check() const;
};
