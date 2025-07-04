// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "InputModeGameplay.generated.h"

USTRUCT(BlueprintType)
struct FInputModeGameplay
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input Mode")
	bool bShowMouseCursor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input Mode")
	bool bFlushInputs;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input Mode")
	bool bConsumeCaptureMouseDown;

	FInputModeGameplay(): bShowMouseCursor(false),
	                      bFlushInputs(false),
	                      bConsumeCaptureMouseDown(false)
	{
	}
};
