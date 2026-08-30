// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UObject/Interface.h"
#include "BetterSettingsCustomOption.generated.h"

UINTERFACE(Blueprintable, BlueprintType)
class UBetterSettingsCustomOption : public UInterface
{
	GENERATED_BODY()
};

class BETTERUI_API IBetterSettingsCustomOption
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Better Settings Custom Option")
	FGameplayTag GetTag() const;
};
