// Copyright VUEDK, Inc. All Rights Reserved.

#include "Utility/BetterDragDropUtility.h"

UBetterDragDropSubsystem* UBetterDragDropUtility::DragDropSubsystem = nullptr;
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

	DragDropSubsystem->SetDragDropOperation(InOperation);
}

void UBetterDragDropUtility::ClearDragDropOperation()
{
	if (!Check())
		return;

	DragDropSubsystem->ClearDragDropOperation();
}

FEventReply UBetterDragDropUtility::DetectBetterDragIfPressed(UUserWidget* InWidget, const FKeyEvent& KeyEvent, const TArray<FKey>& DragKeys)
{
	if (!Check())
		return FEventReply();

	return DragDropSubsystem->DetectBetterDragIfPressed(InWidget, KeyEvent, DragKeys);
}

void UBetterDragDropUtility::DetectBetterDrag(UUserWidget* InWidget)
{
	if (!Check())
		return;

	DragDropSubsystem->DetectBetterDrag(InWidget);
}

void UBetterDragDropUtility::EnterReceiver(UUserWidget* InDropReceiver)
{
	if (!Check())
		return;

	DragDropSubsystem->EnterReceiver(InDropReceiver);
}

void UBetterDragDropUtility::LeaveReceiver()
{
	if (!Check())
		return;

	DragDropSubsystem->LeaveReceiver();
}

bool UBetterDragDropUtility::TryEndDrag()
{
	if (!Check())
		return false;

	return DragDropSubsystem->TryEndDrag();
}

bool UBetterDragDropUtility::IsBetterDragDropping()
{
	if (!Check())
		return false;

	return DragDropSubsystem->IsBetterDragDropping();
}

UBetterDragDropOperation* UBetterDragDropUtility::GetBetterDragDropOperation()
{
	if (!Check())
		return nullptr;
	
	return DragDropSubsystem->GetBetterDragDropOperation();
}

UBetterDragDropSubsystem* UBetterDragDropUtility::GetBetterDragDropSubsystem()
{
	return DragDropSubsystem;
}

bool UBetterDragDropUtility::Check()
{
	return bIsInitialized;
}
