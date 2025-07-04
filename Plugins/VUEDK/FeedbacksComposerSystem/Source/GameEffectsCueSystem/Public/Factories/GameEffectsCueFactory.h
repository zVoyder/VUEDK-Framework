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
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "Instigator"), Category = "GameEffects|Factory")
	static UGameEffectsCue* CreateGameEffectsCue(AActor* Instigator, UGameEffectsCueData* GameEffectsCueData);
};
