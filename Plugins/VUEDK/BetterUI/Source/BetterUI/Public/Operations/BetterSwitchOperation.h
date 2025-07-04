// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/BetterWidget.h"
#include "Components/WidgetSwitcher.h"
#include "UObject/Object.h"
#include "BetterSwitchOperation.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(
	FOnWidgetSwitched,
	int32, OldIndex,
	int32, NewIndex
);

UCLASS(BlueprintType)
class BETTERUI_API UBetterSwitchOperation : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable, Category = Events)
	FOnWidgetSwitched OnWidgetSwitched;

private:
	UPROPERTY()
	UUserWidget* OwnerWidget;
	UPROPERTY()
	UWidgetSwitcher* Switcher;
	UPROPERTY()
	TArray<UBetterWidget*> SelectableWidgets;
	UPROPERTY()
	UBetterWidget* SelectableActiveWidget;

public:
	/**
	 * @brief Initializes the switch operation with the owner widget, switcher, and selectable widgets.
	 * @param InOwnerWidget The widget that owns this operation.
	 * @param InSwitcher The widget switcher to operate on.
	 * @param InSelectableWidgets The list of selectable widgets to manage.
	 */
	void Init(UUserWidget* InOwnerWidget, UWidgetSwitcher* InSwitcher, TArray<UBetterWidget*> InSelectableWidgets);

	/**
	 * @brief Returns the currently active selectable widget.
	 * @return The currently active UBetterWidget.
	 */
	UFUNCTION(BlueprintPure)
	UBetterWidget* GetSelectableActiveWidget() const;

	/**
	 * @brief Returns the currently active widget in the switcher.
	 * @return The currently active UWidget.
	 */
	UFUNCTION(BlueprintPure)
	UWidget* GetSwitcherActiveWidget() const;

	/**
	 * @brief Returns the index of the currently active widget.
	 * @return The index of the active widget.
	 */
	UFUNCTION(BlueprintPure)
	int GetActiveWidgetIndex() const;

	/**
	 * @brief Sets the active widget by index.
	 * @param InIndex The index to set as active.
	 */
	UFUNCTION(BlueprintCallable)
	void SetActiveWidgetIndex(const int32 InIndex);

	/**
	 * @brief Activates the next widget in the switcher.
	 */
	UFUNCTION(BlueprintCallable)
	void ActivateNextWidget();

	/**
	 * @brief Activates the previous widget in the switcher.
	 */
	UFUNCTION(BlueprintCallable)
	void ActivatePreviousWidget();

private:
	/**
	 * @brief Sets up the root for the switch operation.
	 */
	void SetupRoot();

	/**
	 * @brief Binds selection events for the widgets.
	 */
	void BindSelectionEvents();

	/**
	 * @brief Unbinds selection events for the widgets.
	 */
	void UnbindSelectionEvents();

	/**
	 * @brief Checks if the switch operation is valid.
	 * @return True if valid, false otherwise.
	 */
	bool Check() const;

	/**
	 * @brief Handles the selection of a widget.
	 * @param Widget The widget that was selected.
	 */
	UFUNCTION()
	void OnWidgetSelected(UBetterWidget* Widget);

	/**
	 * @brief Handles the deselection of a widget.
	 * @param UserWidget The user widget that was deselected.
	 */
	UFUNCTION()
	void OnOwnerWidgetDestruct(UUserWidget* UserWidget);
};
