// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BetterDragDropOperation.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "BetterDragDropSubsystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
	FBetterDragDropOperationStarted,
	UBetterDragDropOperation*, DragDropOperation
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
	FBetterDragDropOperationEnded,
	UBetterDragDropOperation*, DragDropOperation
);

UCLASS()
class BETTERDRAGSYSTEM_API UBetterDragDropSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FBetterDragDropOperationStarted OnDragDropOperationStarted;
	UPROPERTY(BlueprintAssignable)
	FBetterDragDropOperationEnded OnDragDropOperationEnded;
	
private:
	UPROPERTY()
	UBetterDragDropOperation* CurrentOperation;

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	/**
	 * @brief Sets the current drag and drop operation.
	 * @param InOperation The drag and drop operation to set as current.
	 */
	void SetDragDropOperation(UBetterDragDropOperation* InOperation);

	/**
	 * @brief Clears the current drag and drop operation.
	 */
	void ClearDragDropOperation();

	/**
	 * @brief Detects a BetterDrag operation if a specified key is pressed.
	 * @param InWidget The widget to detect the drag on.
	 * @param KeyEvent The key event to check.
	 * @param DragKeys The keys that can trigger the drag.
	 * @return The event reply indicating if the drag was detected.
	 */
	UFUNCTION(BlueprintCallable, meta = (DefaultToSelf = "InWidget", HidePin = "InWidget"))
	FEventReply DetectBetterDragIfPressed(UUserWidget* InWidget, const FKeyEvent& KeyEvent, const TArray<FKey>& DragKeys) const;

	/**
	 * @brief Detects a BetterDrag operation on the specified widget.
	 * @param InWidget The widget to detect the drag on.
	 */
	UFUNCTION(BlueprintCallable, meta = (DefaultToSelf = "InWidget", HidePin = "InWidget"))
	void DetectBetterDrag(UUserWidget* InWidget) const;

	/**
	 * @brief Notifies that a widget has entered the drop receiver state.
	 * @param InDropReceiver The widget that is now a drop receiver.
	 */
	UFUNCTION(BlueprintCallable)
	void EnterReceiver(UUserWidget* InDropReceiver) const;

	/**
	 * @brief Notifies that the widget has left the drop receiver state.
	 */
	UFUNCTION(BlueprintCallable)
	void LeaveReceiver() const;

	/**
	 * @brief Attempts to end the current drag operation.
	 * @return True if the drag operation ended successfully, false otherwise.
	 */
	UFUNCTION(BlueprintCallable)
	bool TryEndDrag() const;

	/**
	 * @brief Checks if a BetterDrag operation is currently in progress.
	 * @return True if a drag and drop operation is active, false otherwise.
	 */
	UFUNCTION(BlueprintPure)
	bool IsBetterDragDropping() const;

	/**
	 * @brief Gets the current BetterDragDropOperation instance.
	 * @return The current drag and drop operation, or nullptr if none is active.
	 */
	UFUNCTION(BlueprintPure)
	UBetterDragDropOperation* GetBetterDragDropOperation() const;
};
