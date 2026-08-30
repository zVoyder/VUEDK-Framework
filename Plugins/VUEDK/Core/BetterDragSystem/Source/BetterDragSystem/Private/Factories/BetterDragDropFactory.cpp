// Copyright VUEDK, Inc. All Rights Reserved.

#include "BetterDragSystem.h"
#include "Factories/BetterDragDropOperationFactory.h"

UBetterDragDropOperation* UBetterDragAndDropFactory::CreateBetterDragDropOperation(const TSubclassOf<UBetterDragDropOperation> DragDropOperationClass, UUserWidget* OwnerWidget, UUserWidget* DragVisual, UObject* Payload, const ESlateVisibility OwnerWidgetVisibility, const bool bRemoveOnDrag)
{
	if (!IsValid(OwnerWidget) || !IsValid(DragDropOperationClass) || !IsValid(DragVisual))
	{
		UE_LOG(LogBetterDragSystem, Error, TEXT("UBetterDragAndDropFactory::CreateBetterDragDropOperation: Invalid parameters for CreateBetterDragDropOperation."));
		return nullptr;
	}
	
	UBetterDragDropOperation* DragDropOperation = NewObject<UBetterDragDropOperation>(OwnerWidget, DragDropOperationClass);
	DragDropOperation->Init(OwnerWidget, DragVisual, Payload, OwnerWidgetVisibility, bRemoveOnDrag);
	return DragDropOperation;
}

UBetterDragDropOperation* UBetterDragAndDropFactory::CreateBetterDragDropOperationWithKeys(const TSubclassOf<UBetterDragDropOperation> DragDropOperationClass, UUserWidget* OwnerWidget, UUserWidget* DragVisual, UObject* Payload, const ESlateVisibility OwnerWidgetVisibility, const bool bRemoveOnDrag, const TArray<FKey> DropKeys)
{
	if (!IsValid(OwnerWidget) || !IsValid(DragDropOperationClass) || !IsValid(DragVisual))
	{
		UE_LOG(LogBetterDragSystem, Error, TEXT("UBetterDragAndDropFactory::CreateBetterDragDropOperationWithKeys: Invalid parameters for CreateBetterDragDropOperationWithKeys."));
		return nullptr;
	}
	
	UBetterDragDropOperation* DragDropOperation = NewObject<UBetterDragDropOperation>(OwnerWidget, DragDropOperationClass);
	DragDropOperation->Init(OwnerWidget, DragVisual, Payload, OwnerWidgetVisibility, bRemoveOnDrag);
	DragDropOperation->SetDropKeys(DropKeys);
	return DragDropOperation;
}
