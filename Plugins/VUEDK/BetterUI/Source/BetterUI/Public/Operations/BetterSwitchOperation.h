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
	void Init(UUserWidget* InOwnerWidget, UWidgetSwitcher* InSwitcher, TArray<UBetterWidget*> InSelectableWidgets);

	UFUNCTION(BlueprintPure)
	UBetterWidget* GetSelectableActiveWidget() const;

	UFUNCTION(BlueprintPure)
	UWidget* GetSwitcherActiveWidget() const;
	
	UFUNCTION(BlueprintPure)
	int GetActiveWidgetIndex() const;
	
	UFUNCTION(BlueprintCallable)
	void SetActiveWidgetIndex(const int32 InIndex);

	UFUNCTION(BlueprintCallable)
	void ActivateNextWidget();

	UFUNCTION(BlueprintCallable)
	void ActivatePreviousWidget();

private:
	void SetupRoot();
	
	void BindSelectionEvents();

	void UnbindSelectionEvents();
	
	bool Check() const;

	UFUNCTION()
	void OnWidgetSelected(UBetterWidget* Widget);

	UFUNCTION()
	void OnOwnerWidgetDestruct(UUserWidget* UserWidget);
};
