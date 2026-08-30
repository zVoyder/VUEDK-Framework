// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Base/SmartRespawnSelectorBase.h"
#include "SmartRespawnSelectorPriority.generated.h"

UCLASS()
class SMARTRESPAWNSYSTEM_API USmartRespawnSelectorPriority : public USmartRespawnSelectorBase
{
	GENERATED_BODY()

public:
	virtual bool ChoosePlayerRespawn_Implementation(const TMap<FGameplayTag, FSmartRespawnSnapshot>& Respawns, FGameplayTag& OutRespawnTag, FSmartRespawnSnapshot& OutRespawnSnapshot) override;
};
