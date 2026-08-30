// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "BetterDragDropReceiver.generated.h"

class UBetterDragDropOperation;

/**
 * @brief Interface for widgets that can receive BetterDragDrop operations.
 */
UINTERFACE()
class UBetterDragDropReceiver : public UInterface
{
	GENERATED_BODY()
};

class BETTERDRAGSYSTEM_API IBetterDragDropReceiver
{
	GENERATED_BODY()

public:
	/**
	 * @brief Called when a drag and drop operation is dropped on this receiver.
	 * @param DragOperation The drag and drop operation instance.
	 * @param OwnerWidget The widget that owns the drag operation.
	 * @param DragVisual The visual widget displayed during the drag.
	 * @param Payload The payload object associated with the drag operation.
	 */
	UFUNCTION(BlueprintNativeEvent, Category = "Better Drag Drop Receiver")
	void OnBetterDragDrop(UBetterDragDropOperation* DragOperation, UUserWidget* OwnerWidget, UUserWidget* DragVisual, UObject* Payload);
	
	/**
	 * @brief Called when a drag and drop operation enters this receiver.
	 * @param DragOperation The drag and drop operation instance.
	 * @param OwnerWidget The widget that owns the drag operation.
	 * @param DragVisual The visual widget displayed during the drag.
	 * @param Payload The payload object associated with the drag operation.
	 */
	UFUNCTION(BlueprintNativeEvent, Category = "Better Drag Drop Receiver")
	void OnBetterDragEnter(UBetterDragDropOperation* DragOperation, UUserWidget* OwnerWidget, UUserWidget* DragVisual, UObject* Payload);
	
	/**
	 * @brief Called when a drag and drop operation leaves this receiver.
	 * @param DragOperation The drag and drop operation instance.
	 * @param OwnerWidget The widget that owns the drag operation.
	 * @param DragVisual The visual widget displayed during the drag.
	 * @param Payload The payload object associated with the drag operation.
	 */
	UFUNCTION(BlueprintNativeEvent, Category = "Better Drag Drop Receiver")
	void OnBetterDragLeave(UBetterDragDropOperation* DragOperation, UUserWidget* OwnerWidget, UUserWidget* DragVisual, UObject* Payload);
};
