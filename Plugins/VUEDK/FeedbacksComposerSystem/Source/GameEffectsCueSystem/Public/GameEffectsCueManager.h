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
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "Instigator"), Category = "GameEffectsCue")
	static UGameEffectsCue* StartGameEffectsCue(AActor* Instigator, UGameEffectsCueData* GameEffectsCueData);
};
