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
	UFUNCTION(BlueprintCallable, Category = "BetterDrag")
	static UBetterDragDropOperation* CreateBetterDragDropOperation(const TSubclassOf<UBetterDragDropOperation> DragDropOperationClass, UUserWidget* OwnerWidget, UUserWidget* DragVisual, UObject* Payload, const ESlateVisibility OwnerWidgetVisibility);
	
	UFUNCTION(BlueprintCallable, Category = "BetterDrag")
	static UBetterDragDropOperation* CreateBetterDragDropOperationWithKeys(const TSubclassOf<UBetterDragDropOperation> DragDropOperationClass, UUserWidget* OwnerWidget, UUserWidget* DragVisual, UObject* Payload, const ESlateVisibility OwnerWidgetVisibility, TArray<FKey> DropKeys);
};
