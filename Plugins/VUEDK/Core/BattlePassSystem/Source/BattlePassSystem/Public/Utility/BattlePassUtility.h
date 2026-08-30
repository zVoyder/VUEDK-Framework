// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BattlePassSubsystem.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "BattlePassUtility.generated.h"

UCLASS()
class BATTLEPASSSYSTEM_API UBattlePassUtility : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

private:
	static TWeakObjectPtr<UBattlePassSubsystem> BattlePassSubsystem;
	
public:
	static void Init(UBattlePassSubsystem* InBattlePassSubsystem);

	UFUNCTION(BlueprintPure, Category = "Battle Pass")
	static UBattlePass* GetBattlePass(const FGameplayTag BattlePassTag);
};
