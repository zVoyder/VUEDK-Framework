// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "BetterDragDropReceiver.generated.h"

class UBetterDragDropOperation;

UINTERFACE()
class UBetterDragDropReceiver : public UInterface
{
	GENERATED_BODY()
};

class BETTERDRAGSYSTEM_API IBetterDragDropReceiver
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent)
	void OnBetterDragDrop(UBetterDragDropOperation* DragOperation, UUserWidget* OwnerWidget, UUserWidget* DragVisual, UObject* Payload);
	
	UFUNCTION(BlueprintNativeEvent)
	void OnBetterDragEnter(UBetterDragDropOperation* DragOperation, UUserWidget* OwnerWidget, UUserWidget* DragVisual, UObject* Payload);
	
	UFUNCTION(BlueprintNativeEvent)
	void OnBetterDragLeave(UBetterDragDropOperation* DragOperation, UUserWidget* OwnerWidget, UUserWidget* DragVisual, UObject* Payload);
};
