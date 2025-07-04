// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameEffectsCue.h"
#include "Data/GameEffectsCueData.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GameEffectsCueManager.generated.h"

UCLASS()
class GAMEEFFECTSCUESYSTEM_API UGameEffectsCueManager : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/**
	 * Starts a GameEffectsCue with the specified data.
	 * @param Instigator The actor that instigates the effect.
	 * @param GameEffectsCueData The data used to create the GameEffectsCue.
	 * @return A pointer to the started UGameEffectsCue instance, or nullptr if the cue could not be started.
	 */
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "Instigator"), Category = "GameEffectsCue")
	static UGameEffectsCue* StartGameEffectsCue(AActor* Instigator, UGameEffectsCueData* GameEffectsCueData);
};
