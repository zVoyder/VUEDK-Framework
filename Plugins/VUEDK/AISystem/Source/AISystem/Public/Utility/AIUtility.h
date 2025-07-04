// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Perception/AIPerceptionComponent.h"
#include "AIUtility.generated.h"

UCLASS()
class AISYSTEM_API UAIUtility : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/**
	 * @brief Sets the peripheral vision angle for the given AI Perception Component.
	 * @param PerceptionComponent The AI Perception Component to modify.
	 * @param PeripheralVisionAngle The new peripheral vision angle in degrees.
	 */
	UFUNCTION(BlueprintCallable, Category = "VUEDK|AI Utility")
	static void SetPeripheralVisionAngle (UAIPerceptionComponent* PerceptionComponent, float PeripheralVisionAngle);

	/**
	 * @brief Retrieves the sense configuration for a specific sense class from the given AI Perception Component.
	 * @param PerceptionComponent The AI Perception Component to query.
	 * @param SenseClass The class of the sense to retrieve the configuration for.
	 * @return The sense configuration if found, nullptr otherwise.
	 */
	UFUNCTION(BlueprintCallable, Category = "VUEDK|AI Utility")
	static UAISenseConfig* GetSenseConfig(UAIPerceptionComponent* PerceptionComponent, TSubclassOf<UAISense> SenseClass);

	/**
	 * @brief Sets the sight radius for the given AI Perception Component.
	 * @param PerceptionComponent The AI Perception Component to modify.
	 * @param SightRadius The new sight radius value.
	 */
	UFUNCTION(BlueprintCallable, Category = "VUEDK|AI Utility")
	static void SetSightRadius(UAIPerceptionComponent* PerceptionComponent, float SightRadius);

	/**
	 * @brief Sets the lose sight radius for the given AI Perception Component.
	 * @param PerceptionComponent The AI Perception Component to modify.
	 * @param LoseSightRadius The new lose sight radius value.
	 */
	UFUNCTION(BlueprintCallable, Category = "VUEDK|AI Utility")
	static void SetLoseSightRadius(UAIPerceptionComponent* PerceptionComponent, float LoseSightRadius);

	/**
	 * @brief Requests an update for the stimuli listener on the given AI Perception Component.
	 * @param PerceptionComponent The AI Perception Component to update.
	 */
	UFUNCTION(BlueprintCallable, Category = "VUEDK|AI Utility")
	static void RequestStimuliListenerUpdate(UAIPerceptionComponent* PerceptionComponent);
};
