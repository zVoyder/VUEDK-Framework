// Copyright VUEDK, Inc. All Rights Reserved.

#include "Utility/BetterDragDropUtility.h"

TWeakObjectPtr<UBetterDragDropSubsystem> UBetterDragDropUtility::DragDropSubsystem = nullptr;
bool UBetterDragDropUtility::bIsInitialized = false;

void UBetterDragDropUtility::Init(UBetterDragDropSubsystem* InSubsystem)
{
	if (!IsValid(InSubsystem))
	{
		UE_LOG(LogTemp, Error, TEXT("UBetterDragDropUtility::Init: InSubsystem is null"));
		return;
	}

	DragDropSubsystem = InSubsystem;
	bIsInitialized = true;
}

void UBetterDragDropUtility::SetDragDropOperation(UBetterDragDropOperation* InOperation)
{
	if (!Check())
		return;

	DragDropSubsystem.Get()->SetDragDropOperation(InOperation);
}

void UBetterDragDropUtility::ClearDragDropOperation()
{
	if (!Check())
		return;

	DragDropSubsystem.Get()->ClearDragDropOperation();
}

FEventReply UBetterDragDropUtility::DetectBetterDragIfPressed(UUserWidget* InWidget, const FKeyEvent& KeyEvent, const TArray<FKey>& DragKeys)
{
	if (!Check())
		return FEventReply();

	return DragDropSubsystem.Get()->DetectBetterDragIfPressed(InWidget, KeyEvent, DragKeys);
}

void UBetterDragDropUtility::DetectBetterDrag(UUserWidget* InWidget)
{
	if (!Check())
		return;

	DragDropSubsystem.Get()->DetectBetterDrag(InWidget);
}

void UBetterDragDropUtility::EnterReceiver(UUserWidget* InDropReceiver)
{
	if (!Check())
		return;

	DragDropSubsystem.Get()->EnterReceiver(InDropReceiver);
}

void UBetterDragDropUtility::LeaveReceiver()
{
	if (!Check())
		return;

	DragDropSubsystem.Get()->LeaveReceiver();
}

bool UBetterDragDropUtility::TryEndDrag()
{
	if (!Check())
		return false;

	return DragDropSubsystem.Get()->TryEndDrag();
}

bool UBetterDragDropUtility::IsBetterDragDropping()
{
	if (!Check())
		return false;

	return DragDropSubsystem.Get()->IsBetterDragDropping();
}

UBetterDragDropOperation* UBetterDragDropUtility::GetBetterDragDropOperation()
{
	if (!Check())
		return nullptr;
	
	return DragDropSubsystem.Get()->GetBetterDragDropOperation();
}

UBetterDragDropSubsystem* UBetterDragDropUtility::GetBetterDragDropSubsystem()
{
	return DragDropSubsystem.Get();
}

bool UBetterDragDropUtility::Check()
{
	return bIsInitialized && DragDropSubsystem.IsValid();
}
