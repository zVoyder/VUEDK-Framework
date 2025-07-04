// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BetterDragDropOperation.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "BetterDragDropOperationFactory.generated.h"

UCLASS()
class BETTERDRAGSYSTEM_API UBetterDragAndDropFactory : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/**
	 * @brief Creates a new BetterDragDropOperation instance.
	 * @param DragDropOperationClass The class of the drag and drop operation to create.
	 * @param OwnerWidget The widget that owns the drag operation.
	 * @param DragVisual The visual widget to display during the drag.
	 * @param Payload The payload object to associate with the drag operation.
	 * @param OwnerWidgetVisibility The visibility to set for the owner widget during the drag.
	 * @return The created BetterDragDropOperation instance.
	 */
	UFUNCTION(BlueprintCallable, Category = "BetterDrag")
	static UBetterDragDropOperation* CreateBetterDragDropOperation(const TSubclassOf<UBetterDragDropOperation> DragDropOperationClass, UUserWidget* OwnerWidget, UUserWidget* DragVisual, UObject* Payload, const ESlateVisibility OwnerWidgetVisibility);
	
	/**
	 * @brief Creates a new BetterDragDropOperation instance with custom drop keys.
	 * @param DragDropOperationClass The class of the drag and drop operation to create.
	 * @param OwnerWidget The widget that owns the drag operation.
	 * @param DragVisual The visual widget to display during the drag.
	 * @param Payload The payload object to associate with the drag operation.
	 * @param OwnerWidgetVisibility The visibility to set for the owner widget during the drag.
	 * @param DropKeys The array of keys that can trigger the drop action.
	 * @return The created BetterDragDropOperation instance.
	 */
	UFUNCTION(BlueprintCallable, Category = "BetterDrag")
	static UBetterDragDropOperation* CreateBetterDragDropOperationWithKeys(const TSubclassOf<UBetterDragDropOperation> DragDropOperationClass, UUserWidget* OwnerWidget, UUserWidget* DragVisual, UObject* Payload, const ESlateVisibility OwnerWidgetVisibility, TArray<FKey> DropKeys);
};
