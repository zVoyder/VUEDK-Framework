// Copyright VUEDK, Inc. All Rights Reserved.

#include "Objects/SmartRespawnBehaviours/SmartRespawnTranslationBehaviour.h"
#include "GameFramework/GameModeBase.h"

void USmartRespawnTranslationBehaviour::OnRespawnStart_Implementation(const APlayerController* InPlayerController, const FGameplayTag& InRespawnTag, const FSmartRespawnSnapshot& InRespawnSnapshot)
{
	ApplyRespawnSnapshot(InRespawnSnapshot);
	FinishExecute(true);
}
