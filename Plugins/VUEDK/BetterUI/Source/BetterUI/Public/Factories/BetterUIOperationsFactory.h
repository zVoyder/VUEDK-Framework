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
	UFUNCTION(BlueprintCallable, Category = "BetterUI|Factory", meta = (DefaultToSelf = "OwnerWidget", HidePin = "OwnerWidget"))
	static UBetterSwitchOperation* CreateBetterSwichOperation(UUserWidget* OwnerWidget, UWidgetSwitcher* InSwitcher, TArray<UBetterWidget*> InBetterWidgetsSwitchList);

	UFUNCTION(BlueprintCallable, Category = "BetterUI|Factory", meta = (DefaultToSelf = "OwnerWidget", HidePin = "OwnerWidget"))
	static UBetterSwitchOperation* CreateBetterSwichOperationWithContainer(UUserWidget* OwnerWidget, UWidgetSwitcher* InSwitcher, UPanelWidget* InPanelContainer);
};
