// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Base/SmartRespawnBehaviour.h"
#include "SmartRespawnTranslationBehaviour.generated.h"

UCLASS()
class SMARTRESPAWNSYSTEM_API USmartRespawnTranslationBehaviour : public USmartRespawnBehaviour
{
	GENERATED_BODY()

protected:
	virtual void OnRespawnStart_Implementation(const APlayerController* InPlayerController, const FGameplayTag& InRespawnTag, const FSmartRespawnSnapshot& InRespawnSnapshot) override;
};
