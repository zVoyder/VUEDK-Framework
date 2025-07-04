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
	
	void Init(UUserWidget* InOwnerWidget, UUserWidget* InDragVisual, UObject* InPayload, ESlateVisibility InOwnerWidgetVisibility);

	void SetDropKeys(const TArray<FKey>& InDropKeys);
	
	void EnterReceiver(UUserWidget* InDropReceiver);

	void LeaveReceiver();
	
	virtual void Tick(float DeltaTime) override;

	virtual bool IsTickable() const override;

	virtual TStatId GetStatId() const override;

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
