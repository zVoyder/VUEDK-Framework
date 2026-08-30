// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameEffectsCue.h"
#include "Data/GameEffectsCueData.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GameEffectsCueFactory.generated.h"

UCLASS()
class GAMEEFFECTSCUESYSTEM_API UGameEffectsCueFactory : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/**
	 * @brief Creates a GameEffectsCue instance from the provided GameEffectsCueData.
	 * @param Instigator The actor that instigates the effect.
	 * @param GameEffectsCueData The data used to create the GameEffectsCue.
	 * @param Payload Optional additional data that can be passed to the GameEffectsCue for initialization.
	 * @return A pointer to the newly created UGameEffectsCue instance, or nullptr if creation failed.
	 */
	UFUNCTION(BlueprintCallable, meta = (AdvancedDisplay = "Payload"), Category = "GameEffects|Factory")
	static UGameEffectsCue* CreateGameEffectsCue(AActor* Instigator, UGameEffectsCueData* GameEffectsCueData, UObject* Payload = nullptr);

	/**
	 * @brief Creates and starts a GameEffectsCue instance from the provided GameEffectsCueData.
	 * @param Instigator The actor that instigates the effect.
	 * @param GameEffectsCueData The data used to create the GameEffectsCue.
	 * @param Payload Optional additional data that can be passed to the GameEffectsCue for initialization.
	 * @return A pointer to the newly created and started UGameEffectsCue instance, or nullptr if creation failed.
	 */
	UFUNCTION(BlueprintCallable, meta = (AdvancedDisplay = "Payload"), Category = "GameEffectsCue")
	static UGameEffectsCue* StartGameEffectsCue(AActor* Instigator, UGameEffectsCueData* GameEffectsCueData, UObject* Payload = nullptr);
};
