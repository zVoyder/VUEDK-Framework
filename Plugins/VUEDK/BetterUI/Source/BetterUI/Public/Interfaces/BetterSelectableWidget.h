// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "BetterSelectableWidget.generated.h"

UINTERFACE()
class UBetterSelectableWidget : public UInterface
{
	GENERATED_BODY()
};

class BETTERUI_API IBetterSelectableWidget
{
	GENERATED_BODY()

public:
	virtual void NativeSelectWidget(bool bCallEvents) = 0;

	virtual void NativeDeselectWidget(bool bCallEvents) = 0;
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SelectWidget();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void DeselectWidget();
};
