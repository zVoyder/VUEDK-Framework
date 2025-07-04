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
	
	virtual UWidget* GetWidgetFocusTarget_Implementation() override;

	virtual UWidget* GetWidgetUnfocusTarget_Implementation() override;

	virtual void NativeSelectWidget(const bool bCallEvents = true) override;

	virtual void NativeDeselectWidget(const bool bCallEvents = true) override;
	
	virtual void SelectWidget_Implementation() override;

	virtual void DeselectWidget_Implementation() override;

	UFUNCTION(BlueprintPure)
	bool IsBetterWidgetSelected() const;

	UFUNCTION(BlueprintCallable)
	void SetFocusDesiredWidget();

	UFUNCTION(BlueprintCallable)
	void Unfocus();
	
protected:
	UFUNCTION(BlueprintNativeEvent)
	void OnSelectWidget();

	UFUNCTION(BlueprintNativeEvent)
	void OnDeselectWidget();
	
	UFUNCTION(BlueprintNativeEvent)
	bool CanFocus() const;

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
