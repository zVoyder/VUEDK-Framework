// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Interfaces/BetterFocusableWidget.h"
#include "Interfaces/BetterSelectableWidget.h"
#include "BetterWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
	FOnSelected,
	UBetterWidget*, Widget
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
	FOnDeselected,
	UBetterWidget*, Widget
);

UCLASS()
class BETTERUI_API UBetterWidget : public UUserWidget, public IBetterFocusableWidget, public IBetterSelectableWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable, Category = Events)
	FOnSelected OnSelected;
	UPROPERTY(BlueprintAssignable, Category = Events)
	FOnDeselected OnDeselected;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Better UI")
	bool bMouseEnterFocuses = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Better UI")
	bool bMouseLeaveUnfocuses = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Better UI")
	bool bSelectOnAddFocus = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Better UI")
	bool bDeselectOnRemoveFocus = true;

private:
	bool bIsSelected = false;
	
public:
	virtual void NativeConstruct() override;
	
	/**
	 * @brief Returns the widget that should receive focus.
	 * @return The widget to focus.
	 */
	virtual UWidget* GetWidgetFocusTarget_Implementation() override;

	/**
	 * @brief Returns the widget that should lose focus.
	 * @return The widget to unfocus.
	 */
	virtual UWidget* GetWidgetUnfocusTarget_Implementation() override;

	/**
	 * @brief Selects the widget natively, optionally calling events.
	 * @param bCallEvents If true, selection events will be called.
	 */
	virtual void NativeSelectWidget(const bool bCallEvents = true) override;

	/**
	 * @brief Deselects the widget natively, optionally calling events.
	 * @param bCallEvents If true, deselection events will be called.
	 */
	virtual void NativeDeselectWidget(const bool bCallEvents = true) override;
	
	/**
	 * @brief Selects the widget (Blueprint event).
	 */
	virtual void SelectWidget_Implementation() override;

	/**
	 * @brief Deselects the widget (Blueprint event).
	 */
	virtual void DeselectWidget_Implementation() override;

	/**
	 * @brief Returns true if the widget is currently selected.
	 * @return True if selected, false otherwise.
	 */
	UFUNCTION(BlueprintPure)
	bool IsBetterWidgetSelected() const;

	/**
	 * @brief Sets focus to the desired widget.
	 */
	UFUNCTION(BlueprintCallable)
	void SetFocusDesiredWidget();

	/**
	 * @brief Removes focus from the widget.
	 */
	UFUNCTION(BlueprintCallable)
	void Unfocus();
	
protected:
	/**
	 * @brief Called when the widget is selected (Blueprint event).
	 */
	UFUNCTION(BlueprintNativeEvent)
	void OnSelectWidget();

	/**
	 * @brief Called when the widget is deselected (Blueprint event).
	 */
	UFUNCTION(BlueprintNativeEvent)
	void OnDeselectWidget();
	
	/**
	 * @brief Returns true if the widget can be focused.
	 * @return True if can be focused, false otherwise.
	 */
	UFUNCTION(BlueprintNativeEvent)
	bool CanFocus() const;

	/**
	 * @brief Returns true if the widget can be unfocused.
	 * @return True if can be unfocused, false otherwise.
	 */
	UFUNCTION(BlueprintNativeEvent)
	bool CanUnfocus() const;

	virtual FReply NativeOnFocusReceived(const FGeometry& InGeometry, const FFocusEvent& InFocusEvent) override;
	
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;

	virtual void NativeOnAddedToFocusPath(const FFocusEvent& InFocusEvent) override;

	virtual void NativeOnRemovedFromFocusPath(const FFocusEvent& InFocusEvent) override;

private:
	/**
	 * @brief If the RootWidget is a Button set the focus to the widget that should be focused when the button is clicked.
	 */
	void SetButtonFocus() const;
};
