// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BetterDragDropOperation.h"
#include "BetterDragDropSubsystem.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "BetterDragDropUtility.generated.h"

UCLASS()
class BETTERDRAGSYSTEM_API UBetterDragDropUtility : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

private:
	static TWeakObjectPtr<UBetterDragDropSubsystem> DragDropSubsystem;
	static bool bIsInitialized;
	
public:
	/**
	 * @brief Initializes the drag and drop subsystem.
	 * @param InSubsystem The BetterDragDropSubsystem instance to initialize with.
	 */
	static void Init(UBetterDragDropSubsystem* InSubsystem);
	
	/**
	 * @brief Sets the current drag and drop operation.
	 * @param InOperation The drag and drop operation to set.
	 */
	static void SetDragDropOperation(UBetterDragDropOperation* InOperation);

	/**
	 * @brief Clears the current drag and drop operation.
	 */
	static void ClearDragDropOperation();

	/**
	 * @brief Detects a BetterDrag operation if a specified key is pressed.
	 * @param InWidget The widget to detect the drag on.
	 * @param KeyEvent The key event to check.
	 * @param DragKeys The keys that can trigger the drag.
	 * @return The event reply indicating if the drag was detected.
	 */
	UFUNCTION(BlueprintCallable, Category = "BetterDrag",  meta = (DefaultToSelf = "InWidget", HidePin = "InWidget"))
	static FEventReply DetectBetterDragIfPressed(UUserWidget* InWidget, const FKeyEvent& KeyEvent, const TArray<FKey>& DragKeys);

	/**
	 * @brief Detects a BetterDrag operation on the specified widget.
	 * @param InWidget The widget to detect the drag on.
	 */
	UFUNCTION(BlueprintCallable, Category = "BetterDrag", meta = (DefaultToSelf = "InWidget", HidePin = "InWidget"))
	static void DetectBetterDrag(UUserWidget* InWidget);
	
	/**
	 * @brief Notifies that a widget has entered the drop receiver state.
	 * @param InDropReceiver The widget that is now a drop receiver.
	 */
	UFUNCTION(BlueprintCallable, Category = "BetterDrag")
	static void EnterReceiver(UUserWidget* InDropReceiver);

	/**
	 * @brief Notifies that the widget has left the drop receiver state.
	 */
	UFUNCTION(BlueprintCallable, Category = "BetterDrag")
	static void LeaveReceiver();

	/**
	 * @brief Attempts to end the current drag operation.
	 * @return True if the drag operation ended successfully, false otherwise.
	 */
	UFUNCTION(BlueprintCallable, Category = "BetterDrag")
	static bool TryEndDrag();

	/**
	 * @brief Checks if a BetterDrag operation is currently in progress.
	 * @return True if a drag and drop operation is active, false otherwise.
	 */
	UFUNCTION(BlueprintPure, Category = "BetterDrag")
	static bool IsBetterDragDropping();

	/**
	 * @brief Gets the current BetterDragDropOperation instance.
	 * @return The current drag and drop operation, or nullptr if none is active.
	 */
	UFUNCTION(BlueprintPure, Category = "BetterDrag")
	static UBetterDragDropOperation* GetBetterDragDropOperation();

	/**
	 * @brief Gets the current BetterDragDropSubsystem instance.
	 * @return The current drag and drop subsystem, or nullptr if not initialized.
	 */
	UFUNCTION(BlueprintPure, Category = "BetterDrag")
	static UBetterDragDropSubsystem* GetBetterDragDropSubsystem();

private:
	static bool Check();
};
