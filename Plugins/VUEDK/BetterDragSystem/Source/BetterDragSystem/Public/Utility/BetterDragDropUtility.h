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
	static UBetterDragDropSubsystem* DragDropSubsystem;
	static bool bIsInitialized;
	
public:
	static void Init(UBetterDragDropSubsystem* InSubsystem);
	
	static void SetDragDropOperation(UBetterDragDropOperation* InOperation);

	static void ClearDragDropOperation();

	UFUNCTION(BlueprintCallable, Category = "BetterDrag",  meta = (DefaultToSelf = "InWidget", HidePin = "InWidget"))
	static FEventReply DetectBetterDragIfPressed(UUserWidget* InWidget, const FKeyEvent& KeyEvent, const TArray<FKey>& DragKeys);

	UFUNCTION(BlueprintCallable, Category = "BetterDrag", meta = (DefaultToSelf = "InWidget", HidePin = "InWidget"))
	static void DetectBetterDrag(UUserWidget* InWidget);
	
	UFUNCTION(BlueprintCallable, Category = "BetterDrag")
	static void EnterReceiver(UUserWidget* InDropReceiver);

	UFUNCTION(BlueprintCallable, Category = "BetterDrag")
	static void LeaveReceiver();

	UFUNCTION(BlueprintCallable, Category = "BetterDrag")
	static bool TryEndDrag();

	UFUNCTION(BlueprintPure, Category = "BetterDrag")
	static bool IsBetterDragDropping();

	UFUNCTION(BlueprintPure, Category = "BetterDrag")
	static UBetterDragDropOperation* GetBetterDragDropOperation();

	UFUNCTION(BlueprintPure, Category = "BetterDrag")
	static UBetterDragDropSubsystem* GetBetterDragDropSubsystem();

private:
	static bool Check();
};
