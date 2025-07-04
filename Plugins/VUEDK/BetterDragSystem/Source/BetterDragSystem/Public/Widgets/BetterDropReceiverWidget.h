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

	UFUNCTION(BlueprintNativeEvent, BlueprintPure)
	bool CanReceiveDrop() const;

	virtual FReply NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;

private:
	UFUNCTION()
	void OnDragDropOperationEnded(UBetterDragDropOperation* DragDropOperation);
};
