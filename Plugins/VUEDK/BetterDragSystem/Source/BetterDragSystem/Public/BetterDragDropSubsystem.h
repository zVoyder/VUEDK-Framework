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

	void SetDragDropOperation(UBetterDragDropOperation* InOperation);

	void ClearDragDropOperation();

	UFUNCTION(BlueprintCallable, meta = (DefaultToSelf = "InWidget", HidePin = "InWidget"))
	FEventReply DetectBetterDragIfPressed(UUserWidget* InWidget, const FKeyEvent& KeyEvent, const TArray<FKey>& DragKeys) const;

	UFUNCTION(BlueprintCallable, meta = (DefaultToSelf = "InWidget", HidePin = "InWidget"))
	void DetectBetterDrag(UUserWidget* InWidget) const;

	UFUNCTION(BlueprintCallable)
	void EnterReceiver(UUserWidget* InDropReceiver) const;

	UFUNCTION(BlueprintCallable)
	void LeaveReceiver() const;

	UFUNCTION(BlueprintCallable)
	bool TryEndDrag() const;

	UFUNCTION(BlueprintPure)
	bool IsBetterDragDropping() const;

	UFUNCTION(BlueprintPure)
	UBetterDragDropOperation* GetBetterDragDropOperation() const;
};
