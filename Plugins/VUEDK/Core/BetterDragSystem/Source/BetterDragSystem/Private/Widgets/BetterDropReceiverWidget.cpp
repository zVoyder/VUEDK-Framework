// Copyright VUEDK, Inc. All Rights Reserved.

#include "Widgets/BetterDropReceiverWidget.h"
#include "Utility/BetterDragDropUtility.h"

void UBetterDropReceiverWidget::NativeConstruct()
{
	Super::NativeConstruct();

	DragDropSubsystem = UBetterDragDropUtility::GetBetterDragDropSubsystem();

	if (!IsValid(DragDropSubsystem))
	{
		UE_LOG(LogTemp, Error, TEXT("UBetterDropReceiverWidget::NativeConstruct: DragDropSubsystem is null"));
		return;
	}

	DragDropSubsystem->OnDragDropOperationEnded.AddDynamic(this, &UBetterDropReceiverWidget::OnDragDropOperationEnded);
}

void UBetterDropReceiverWidget::NativeDestruct()
{
	Super::NativeDestruct();

	if (!IsValid(DragDropSubsystem))
		return;

	DragDropSubsystem->OnDragDropOperationEnded.RemoveDynamic(this, &UBetterDropReceiverWidget::OnDragDropOperationEnded);
}

bool UBetterDropReceiverWidget::CanReceiveDrop_Implementation() const
{
	return true;
}

FReply UBetterDropReceiverWidget::NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (bIsMouseEntered || !UBetterDragDropUtility::IsBetterDragDropping() || !CanReceiveDrop())
		return Super::NativeOnMouseMove(InGeometry, InMouseEvent);

	bIsMouseEntered = true;
	UBetterDragDropUtility::EnterReceiver(this);
	return Super::NativeOnMouseMove(InGeometry, InMouseEvent);
}

void UBetterDropReceiverWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);
	UBetterDragDropUtility::LeaveReceiver();
	bIsMouseEntered = false;
}

void UBetterDropReceiverWidget::OnDragDropOperationEnded(UBetterDragDropOperation* DragDropOperation)
{
	bIsMouseEntered = false;
}
