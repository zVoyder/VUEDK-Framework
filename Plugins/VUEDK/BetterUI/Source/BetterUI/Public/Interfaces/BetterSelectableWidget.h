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
	/**
	 * @brief Selects the widget natively, optionally calling events.
	 * @param bCallEvents If true, selection events will be called.
	 */
	virtual void NativeSelectWidget(bool bCallEvents) = 0;

	/**
	 * @brief Deselects the widget natively, optionally calling events.
	 * @param bCallEvents If true, deselection events will be called.
	 */
	virtual void NativeDeselectWidget(bool bCallEvents) = 0;
	
	/**
	 * @brief Selects the widget (Blueprint event).
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SelectWidget();

	/**
	 * @brief Deselects the widget (Blueprint event).
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void DeselectWidget();
};
