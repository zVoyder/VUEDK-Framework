// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "InputModeUI.generated.h"

USTRUCT(BlueprintType)
struct FInputModeUI
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input Mode")
	bool bShowMouseCursor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input Mode")
	bool bFlushInputs;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input Mode")
	bool bHideCursorDuringCapture;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input Mode")
	bool bCenterMouseOnViewport;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input Mode")
	EMouseLockMode LockMouseToViewportBehavior;
	
	FInputModeUI(): bShowMouseCursor(true),
	                bFlushInputs(true),
	                bHideCursorDuringCapture(false),
	                bCenterMouseOnViewport(true),
	                LockMouseToViewportBehavior(EMouseLockMode::DoNotLock)
	{
	}
};
