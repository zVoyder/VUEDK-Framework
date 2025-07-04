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
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	UWidget* GetWidgetFocusTarget();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	UWidget* GetWidgetUnfocusTarget();
};
