// Copyright VUEDK, Inc. All Rights Reserved.

#include "Factories/BetterUIOperationsFactory.h"
#include "Operations/BetterSwitchOperation.h"
#include "BetterUI.h"

UBetterSwitchOperation* UBetterUIOperationsFactory::CreateBetterSwichOperation(UUserWidget* OwnerWidget, UWidgetSwitcher* InSwitcher, TArray<UBetterWidget*> InBetterWidgetsSwitchList)
{
	if (!IsValid(OwnerWidget) || !IsValid(InSwitcher) || InBetterWidgetsSwitchList.IsEmpty())
	{
		UE_LOG(LogBetterUI, Error, TEXT("UBetterUIPanelFactory::CreateBetterSwichOperation: Invalid parameters for CreateBetterSwichOperation."));
		return nullptr;
	}

	UBetterSwitchOperation* SwitchOperation = NewObject<UBetterSwitchOperation>(OwnerWidget);
	SwitchOperation->Init(OwnerWidget, InSwitcher, InBetterWidgetsSwitchList);
	return SwitchOperation;
}

UBetterSwitchOperation* UBetterUIOperationsFactory::CreateBetterSwitchOperationWithContainer(UUserWidget* OwnerWidget, UWidgetSwitcher* InSwitcher, UPanelWidget* InPanelContainer)
{
	if (!IsValid(OwnerWidget) || !IsValid(InSwitcher) || !IsValid(InPanelContainer))
	{
		UE_LOG(LogBetterUI, Error, TEXT("UBetterUIPanelFactory::CreateBetterSwitchOperationWithContainer: Invalid parameters for CreateBetterSwichOperationWithContainer."));
		return nullptr;
	}
	
	TArray<UBetterWidget*> BetterWidgets;
	for (UWidget* Widget : InPanelContainer->GetAllChildren())
	{
		if (UBetterWidget* BetterWidget = Cast<UBetterWidget>(Widget))
			BetterWidgets.Add(BetterWidget);
	}

	if (BetterWidgets.IsEmpty())
	{
		UE_LOG(LogBetterUI, Error, TEXT("UBetterUIPanelFactory::CreateBetterSwitchOperationWithContainer: No BetterWidgets found in the container."));
		return nullptr;
	}
	
	UBetterSwitchOperation* SwitchOperation = NewObject<UBetterSwitchOperation>(OwnerWidget);
	SwitchOperation->Init(OwnerWidget, InSwitcher, BetterWidgets);
	return SwitchOperation;
}
