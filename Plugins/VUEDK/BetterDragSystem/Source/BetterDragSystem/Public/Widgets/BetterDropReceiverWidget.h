// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Interfaces/BetterDragDropReceiver.h"
#include "BetterDropReceiverWidget.generated.h"

class UBetterDragDropSubsystem;

UCLASS(BlueprintType, Blueprintable)
class BETTERDRAGSYSTEM_API UBetterDropReceiverWidget : public UUserWidget, public IBetterDragDropReceiver
{
	GENERATED_BODY()

private:
	UPROPERTY()
	UBetterDragDropSubsystem* DragDropSubsystem;
	bool bIsMouseEntered = false;

public:
	virtual void NativeConstruct() override;
	
	virtual void NativeDestruct() override;

	/**
	 * @brief Determines if this widget can currently receive a drop operation.
	 * @return True if the widget can receive a drop, false otherwise.
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintPure)
	bool CanReceiveDrop() const;

	/**
	 * @brief Handles mouse move events for drag and drop logic.
	 * @param InGeometry The geometry of the widget.
	 * @param InMouseEvent The mouse event data.
	 * @return The reply to the mouse move event.
	 */
	virtual FReply NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	/**
	 * @brief Handles mouse leave events for drag and drop logic.
	 * @param InMouseEvent The mouse event data.
	 */
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;

private:
	/**
	 * @brief Called when a drag and drop operation ends on this widget.
	 * @param DragDropOperation The drag and drop operation that ended.
	 */
	UFUNCTION()
	void OnDragDropOperationEnded(UBetterDragDropOperation* DragDropOperation);
};
