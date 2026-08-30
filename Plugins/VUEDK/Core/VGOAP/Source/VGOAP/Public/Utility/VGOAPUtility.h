// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GOAPBlackboardsSubsystem.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "VGOAPUtility.generated.h"

UCLASS()
class VGOAP_API UVGOAPUtility : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

private:
	static TWeakObjectPtr<UGOAPBlackboardsSubsystem> BlackboardsSubsystem;

public:
	static void Init(UGOAPBlackboardsSubsystem* InBlackboardsSubsystem);

	UFUNCTION(BlueprintPure, Category = "VGOAP|Utility")
	static UGOAPBlackboard* GetGlobalBlackboard(const FGameplayTag BlackboardTag);
};
