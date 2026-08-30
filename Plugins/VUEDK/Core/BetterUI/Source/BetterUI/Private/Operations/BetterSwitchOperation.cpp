// Copyright VUEDK, Inc. All Rights Reserved.

#include "Operations/BetterSwitchOperation.h"
#include "BetterUI.h"
#include "Interfaces/BetterSwitcherObject.h"
#include "Kismet/KismetSystemLibrary.h"

void UBetterSwitchOperation::Init(UUserWidget* InOwnerWidget, UWidgetSwitcher* InSwitcher, TArray<UBetterWidget*> InSelectableWidgets)
{
	OwnerWidget = InOwnerWidget;
	Switcher = InSwitcher;
	SelectableWidgets = InSelectableWidgets;
	SetupRoot();
	BindSelectionEvents();
}

UBetterWidget* UBetterSwitchOperation::GetSelectableActiveWidget() const
{
	return SelectableActiveWidget;
}

UWidget* UBetterSwitchOperation::GetSwitcherActiveWidget() const
{
	if (!Check())
		return nullptr;

	return Switcher->GetActiveWidget();
}

int UBetterSwitchOperation::GetActiveWidgetIndex() const
{
	if (!Check())
		return INDEX_NONE;

	return Switcher->GetActiveWidgetIndex();
}

void UBetterSwitchOperation::SetActiveWidgetIndex(const int32 InIndex)
{
	if (!Check())
		return;
	
	if (!Switcher->GetAllChildren().IsValidIndex(InIndex))
	{
		UE_LOG(LogBetterUI, Warning, TEXT("UBetterSwitchOperation::SetActiveWidgetIndex: Invalid index %d"), InIndex);
		return;
	}

	const int32 OldIndex = Switcher->GetActiveWidgetIndex();
	Switcher->SetActiveWidgetIndex(InIndex);

	UWidget* SwitcherNewWidget = GetSwitcherActiveWidget();
	if (IsValid(SwitcherNewWidget) && UKismetSystemLibrary::DoesImplementInterface(SwitcherNewWidget, UBetterSwitcherObject::StaticClass()))
		IBetterSwitcherObject::Execute_SwitcherActivatedWidget(SwitcherNewWidget);

	if (SelectableWidgets.IsValidIndex(InIndex))
	{
		UBetterWidget* Widget = SelectableWidgets[InIndex];
		Widget->NativeSelectWidget(false);

		if (IsValid(SelectableActiveWidget))
			SelectableActiveWidget->NativeDeselectWidget();

		SelectableActiveWidget = Widget;
	}

	if (!Switcher->IsDesignTime())
		OnWidgetSwitched.Broadcast(OldIndex, InIndex);
}

void UBetterSwitchOperation::ActivateNextWidget()
{
	if (!Check())
		return;

	const int32 NextIndex = Switcher->GetActiveWidgetIndex() + 1;
	const TArray<UWidget*> Children = Switcher->GetAllChildren();

	if (Children.IsValidIndex(NextIndex))
		SetActiveWidgetIndex(NextIndex);
	else
		SetActiveWidgetIndex(0);
}

void UBetterSwitchOperation::ActivatePreviousWidget()
{
	if (!Check())
		return;

	const int32 PreviousIndex = Switcher->GetActiveWidgetIndex() - 1;
	const TArray<UWidget*> Children = Switcher->GetAllChildren();

	if (Children.IsValidIndex(PreviousIndex))
		SetActiveWidgetIndex(PreviousIndex);
	else
		SetActiveWidgetIndex(Children.Num() - 1);
}

void UBetterSwitchOperation::SetupRoot()
{
	if (!Check())
		return;

	AddToRoot();
	OwnerWidget->OnNativeDestruct.AddUObject(this, &UBetterSwitchOperation::OnOwnerWidgetDestruct);
}

void UBetterSwitchOperation::BindSelectionEvents()
{
	if (!Check())
		return;

	for (UBetterWidget* Widget : SelectableWidgets)
		Widget->OnSelected.AddDynamic(this, &UBetterSwitchOperation::OnWidgetSelected);
}

void UBetterSwitchOperation::UnbindSelectionEvents()
{
	for (UBetterWidget* Widget : SelectableWidgets)
		Widget->OnSelected.RemoveDynamic(this, &UBetterSwitchOperation::OnWidgetSelected);
}

bool UBetterSwitchOperation::Check() const
{
	return IsValid(OwnerWidget) && IsValid(Switcher) && !SelectableWidgets.IsEmpty();
}

void UBetterSwitchOperation::OnWidgetSelected(UBetterWidget* Widget)
{
	const int32 Index = SelectableWidgets.IndexOfByKey(Widget);
	SetActiveWidgetIndex(Index);
}

void UBetterSwitchOperation::OnOwnerWidgetDestruct(UUserWidget* UserWidget)
{
	UnbindSelectionEvents();
	OwnerWidget->OnNativeDestruct.RemoveAll(this);
	RemoveFromRoot();
}
