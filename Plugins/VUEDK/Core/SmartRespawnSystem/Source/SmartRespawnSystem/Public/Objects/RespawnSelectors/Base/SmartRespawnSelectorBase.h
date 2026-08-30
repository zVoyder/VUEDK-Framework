// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "SmartRespawnSubsystem.h"
#include "Data/SmartRespawnSnapshot.h"
#include "UObject/Object.h"
#include "SmartRespawnSelectorBase.generated.h"

UCLASS(Abstract, Blueprintable, BlueprintType)
class SMARTRESPAWNSYSTEM_API USmartRespawnSelectorBase : public USmartRespawnObjectBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent)
	bool ChoosePlayerRespawn(const TMap<FGameplayTag, FSmartRespawnSnapshot>& Respawns, FGameplayTag& OutRespawnTag, FSmartRespawnSnapshot& OutRespawnSnapshot);

protected:
	UFUNCTION(BlueprintCallable)
	static bool FindBestRespawnByPriority(const TMap<FGameplayTag, FSmartRespawnSnapshot>& Respawns, FGameplayTag& OutRespawnTag, FSmartRespawnSnapshot& OutRespawnSnapshot);

	UFUNCTION(BlueprintCallable)
	static bool FindBestRespawnMostRecent(const TMap<FGameplayTag, FSmartRespawnSnapshot>& Respawns, FGameplayTag& OutRespawnTag, FSmartRespawnSnapshot& OutRespawnSnapshot);
};
