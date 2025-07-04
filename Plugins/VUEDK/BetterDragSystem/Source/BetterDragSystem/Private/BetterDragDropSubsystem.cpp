// Copyright VUEDK, Inc. All Rights Reserved.

#include "BetterDragDropSubsystem.h"
#include "Utility/BetterDragDropUtility.h"

void UBetterDragDropSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	UBetterDragDropUtility::Init(this);
}

void UBetterDragDropSubsystem::SetDragDropOperation(UBetterDragDropOperation* InOperation)
{
	if (InOperation == nullptr)
		return;
	
	CurrentOperation = InOperation;
	OnDragDropOperationStarted.Broadcast(CurrentOperation);
}

void UBetterDragDropSubsystem::ClearDragDropOperation()
{
	OnDragDropOperationEnded.Broadcast(CurrentOperation);
	CurrentOperation = nullptr;
}

FEventReply UBetterDragDropSubsystem::DetectBetterDragIfPressed(UUserWidget* InWidget, const FKeyEvent& KeyEvent, const TArray<FKey>& DragKeys) const
{
	FEventReply Reply;
	if (IsBetterDragDropping() || !IsValid(InWidget) || !DragKeys.Contains(KeyEvent.GetKey()))
	{
		Reply.NativeReply = FReply::Unhandled();
		return Reply;
	}
	
	UDragDropOperation* Operation = nullptr;
	InWidget->OnDragDetected(InWidget->GetCachedGeometry(), FPointerEvent(), Operation);
	Reply.NativeReply = FReply::Handled();
	return Reply;
}

void UBetterDragDropSubsystem::DetectBetterDrag(UUserWidget* InWidget) const
{
	if (IsBetterDragDropping() || !IsValid(InWidget))
		return;

	UDragDropOperation* Operation = nullptr;
	InWidget->OnDragDetected(InWidget->GetCachedGeometry(), FPointerEvent(), Operation);
}

void UBetterDragDropSubsystem::EnterReceiver(UUserWidget* InDropReceiver) const
{
	if (!IsBetterDragDropping())
		return;
	
	CurrentOperation->EnterReceiver(InDropReceiver);
}

void UBetterDragDropSubsystem::LeaveReceiver() const
{
	if (!IsBetterDragDropping())
		return;
	
	CurrentOperation->LeaveReceiver();
}

bool UBetterDragDropSubsystem::TryEndDrag() const
{
	if (!IsBetterDragDropping())
		return false;
	
	CurrentOperation->CheckEndDrag();
	return true;
}

bool UBetterDragDropSubsystem::IsBetterDragDropping() const
{
	return IsValid(CurrentOperation);
}

UBetterDragDropOperation* UBetterDragDropSubsystem::GetBetterDragDropOperation() const
{
	return CurrentOperation;
}
