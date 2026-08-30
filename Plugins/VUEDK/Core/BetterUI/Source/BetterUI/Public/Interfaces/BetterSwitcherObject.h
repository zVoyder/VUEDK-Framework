// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "BetterSwitcherObject.generated.h"

UINTERFACE()
class UBetterSwitcherObject : public UInterface
{
	GENERATED_BODY()
};

class BETTERUI_API IBetterSwitcherObject
{
	GENERATED_BODY()

public:
	/**
	 * @brief Called when the switcher activates this widget.
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Better Switcher Object")
	void SwitcherActivatedWidget();
};
