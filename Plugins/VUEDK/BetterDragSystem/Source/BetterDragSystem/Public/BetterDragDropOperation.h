// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/BetterDropReceiverWidget.h"
#include "BetterDragDropOperation.generated.h"


UCLASS(Blueprintable, BlueprintType)
class BETTERDRAGSYSTEM_API UBetterDragDropOperation : public UObject, public FTickableGameObject
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly)
	UUserWidget* OwnerWidget;
	UPROPERTY(BlueprintReadOnly)
	UUserWidget* DragVisual;
	UPROPERTY(BlueprintReadOnly)
	UObject* Payload;
	UPROPERTY(BlueprintReadOnly)
	APlayerController* PlayerController;
	UPROPERTY(BlueprintReadOnly)
    UUserWidget* DropReceiver;
	
private:
	ESlateVisibility OwnerWidgetVisibility;
	ESlateVisibility CachedOwnerWidgetVisibility;
	TArray<FKey> DropKeys;
	bool bIsDragging;
	FVector2D CachedSize;
	TArray<FInputKeyBinding> DropKeysBindings;

public:
	UBetterDragDropOperation();
	
	/**
	 * @brief Initializes the drag and drop operation with the specified parameters.
	 * @param InOwnerWidget The widget that owns the drag operation.
	 * @param InDragVisual The visual widget to display during the drag.
	 * @param InPayload The payload object for the drag operation.
	 * @param InOwnerWidgetVisibility The visibility to set for the owner widget during the drag.
	 */
	void Init(UUserWidget* InOwnerWidget, UUserWidget* InDragVisual, UObject* InPayload, ESlateVisibility InOwnerWidgetVisibility);

	/**
	 * @brief Sets the keys that can trigger the drop action.
	 * @param InDropKeys The array of keys for dropping.
	 */
	void SetDropKeys(const TArray<FKey>& InDropKeys);

	/**
	 * @brief Called when the drag operation enters a drop receiver widget.
	 * @param InDropReceiver The widget that is now the drop receiver.
	 */
	void EnterReceiver(UUserWidget* InDropReceiver);

	/**
	 * @brief Called when the drag operation leaves a drop receiver widget.
	 */
	void LeaveReceiver();

	/**
	 * @brief Updates the drag operation every frame.
	 * @param DeltaTime The time elapsed since the last tick.
	 */
	virtual void Tick(float DeltaTime) override;

	/**
	 * @brief Checks if the drag operation is tickable.
	 * @return True if tickable, false otherwise.
	 */
	virtual bool IsTickable() const override;

	/**
	 * @brief Gets the stat ID for this tickable object.
	 * @return The stat ID.
	 */
	virtual TStatId GetStatId() const override;
	
	/**
	 * @brief Checks if the drag operation should end.
	 * @return True if the drag should end, false otherwise.
	 */
	UFUNCTION(BlueprintCallable)
	bool CheckEndDrag();
	
	UFUNCTION(BlueprintCallable)
	bool CheckEndDragIfReleased(FKey Key);
	
protected:
	UFUNCTION(BlueprintNativeEvent)
	void UpdateDrag(const FVector2D Size);

	UFUNCTION(BlueprintNativeEvent)
	void SetDragVisualPosition(const FVector2D PositionInViewport) const;

	UFUNCTION(BlueprintNativeEvent)
	void OnBeginDrag();

	UFUNCTION(BlueprintNativeEvent)
	void OnEndDrag();

	UFUNCTION(BlueprintNativeEvent)
	void OnDragEnter(UUserWidget* EnteredWidget);

	UFUNCTION(BlueprintNativeEvent)
	void OnDragLeave(UUserWidget* LeftWidget);

	/**
	 * @brief Called when the drag operation is dropped.
	 * @return True if the drag operation is confirmed, false otherwise.
	 */
	UFUNCTION(BlueprintNativeEvent)
	bool OnDragDrop();
	
	UFUNCTION(BlueprintNativeEvent)
	void OnDragCancelled();

	virtual UWorld* GetWorld() const override;

#if WITH_EDITOR
	virtual bool ImplementsGetWorld() const override;
#endif
	
private:
	void BeginDrag();
	
	void EndDrag();

	UFUNCTION()
	void CheckEndDragKeyEvent(FKey Key);

	void BindDropKeysEvent();

	void UnbindDropKeysEvent();
	
	bool Check() const;
};
