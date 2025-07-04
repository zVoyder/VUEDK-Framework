// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/Widget.h"
#include "UObject/Interface.h"
#include "BetterFocusableWidget.generated.h"

UINTERFACE()
class UBetterFocusableWidget : public UInterface
{
	GENERATED_BODY()
};

class BETTERUI_API IBetterFocusableWidget
{
	GENERATED_BODY()
	
public:
	/**
	 * @brief Returns the widget that should receive focus.
	 * @return The widget to focus.
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	UWidget* GetWidgetFocusTarget();

	/**
	 * @brief Returns the widget that should lose focus.
	 * @return The widget to unfocus.
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	UWidget* GetWidgetUnfocusTarget();
};


