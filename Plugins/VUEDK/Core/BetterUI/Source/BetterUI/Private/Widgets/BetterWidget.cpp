// Copyright VUEDK, Inc. All Rights Reserved.

#include "Widgets/BetterWidget.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Components/Button.h"

void UBetterWidget::NativeConstruct()
{
	Super::NativeConstruct();
	SetButtonFocus();
}

UWidget* UBetterWidget::GetWidgetFocusTarget_Implementation()
{
	return this;
}

UWidget* UBetterWidget::GetWidgetUnfocusTarget_Implementation()
{
	return nullptr;
}

void UBetterWidget::NativeSelectWidget(const bool bCallEvents)
{
	if (bIsSelected)
		return;

	bIsSelected = true;
	OnSelectWidget();
	if (bCallEvents)
		OnSelected.Broadcast(this);
}

void UBetterWidget::NativeDeselectWidget(const bool bCallEvents)
{
	if (!bIsSelected)
		return;
	
	bIsSelected = false;
	OnDeselectWidget();
	if (bCallEvents)
		OnDeselected.Broadcast(this);
}

void UBetterWidget::SelectWidget_Implementation()
{
	NativeSelectWidget();
}

void UBetterWidget::DeselectWidget_Implementation()
{
	NativeDeselectWidget();
}

bool UBetterWidget::IsBetterWidgetSelected() const
{
	return bIsSelected;
}

void UBetterWidget::SetFocusDesiredWidget()
{
	if (!CanFocus())
		return;

	UWidget* DesiredWidget = IBetterFocusableWidget::Execute_GetWidgetFocusTarget(this);

	if (!IsValid(DesiredWidget))
		return;

	DesiredWidget->SetFocus();
}

void UBetterWidget::Unfocus()
{
	if (!CanUnfocus())
		return;

	UWidget* UnfocusWidget = IBetterFocusableWidget::Execute_GetWidgetUnfocusTarget(this);
	if (!IsValid(UnfocusWidget))
	{
		UWidgetBlueprintLibrary::SetFocusToGameViewport();
		return;
	}

	UnfocusWidget->SetFocus();
}

void UBetterWidget::OnSelectWidget_Implementation()
{
}

void UBetterWidget::OnDeselectWidget_Implementation()
{
}

bool UBetterWidget::CanFocus_Implementation() const
{
	return true;
}

bool UBetterWidget::CanUnfocus_Implementation() const
{
	return true;
}

FReply UBetterWidget::NativeOnFocusReceived(const FGeometry& InGeometry, const FFocusEvent& InFocusEvent)
{
	SetFocusDesiredWidget();
	return Super::NativeOnFocusReceived(InGeometry, InFocusEvent);
}

void UBetterWidget::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);
	if (bMouseEnterFocuses)
		SetFocusDesiredWidget();
}

void UBetterWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);
	if (bMouseLeaveUnfocuses)
		Unfocus();
}

void UBetterWidget::NativeOnAddedToFocusPath(const FFocusEvent& InFocusEvent)
{
	Super::NativeOnAddedToFocusPath(InFocusEvent);

	if (bSelectOnAddFocus)
		NativeSelectWidget();
}

void UBetterWidget::NativeOnRemovedFromFocusPath(const FFocusEvent& InFocusEvent)
{
	Super::NativeOnRemovedFromFocusPath(InFocusEvent);

	if (bDeselectOnRemoveFocus)
		NativeDeselectWidget();
}

void UBetterWidget::SetButtonFocus() const
{
	UWidget* Widget = GetRootWidget();
	if (!IsValid(Widget))
		return;

	UButton* Button = Cast<UButton>(Widget);
	if (!IsValid(Button))
		return;

	Button->OnClicked.AddDynamic(this, &UBetterWidget::SetFocusDesiredWidget);
}
