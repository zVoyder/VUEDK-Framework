// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/BetterWidget.h"
#include "Operations/BetterSwitchOperation.h"
#include "Components/WidgetSwitcher.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "BetterUIOperationsFactory.generated.h"

UCLASS()
class BETTERUI_API UBetterUIOperationsFactory : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/**
	 * @brief Creates a BetterSwitchOperation for the specified switcher and widgets list.
	 * @param OwnerWidget The widget that owns the operation.
	 * @param InSwitcher The widget switcher to operate on.
	 * @param InBetterWidgetsSwitchList The list of BetterWidgets to manage switching.
	 * @return The created BetterSwitchOperation instance.
	 */
	UFUNCTION(BlueprintCallable, Category = "BetterUI|Factory", meta = (DefaultToSelf = "OwnerWidget", HidePin = "OwnerWidget"))
	static UBetterSwitchOperation* CreateBetterSwichOperation(UUserWidget* OwnerWidget, UWidgetSwitcher* InSwitcher, TArray<UBetterWidget*> InBetterWidgetsSwitchList);

	/**
	 * @brief Creates a BetterSwitchOperation for the specified switcher and panel container.
	 * @param OwnerWidget The widget that owns the operation.
	 * @param InSwitcher The widget switcher to operate on.
	 * @param InPanelContainer The panel container holding the widgets to switch between.
	 * @return The created BetterSwitchOperation instance.
	 */
	UFUNCTION(BlueprintCallable, Category = "BetterUI|Factory", meta = (DefaultToSelf = "OwnerWidget", HidePin = "OwnerWidget"))
	static UBetterSwitchOperation* CreateBetterSwichOperationWithContainer(UUserWidget* OwnerWidget, UWidgetSwitcher* InSwitcher, UPanelWidget* InPanelContainer);
};
