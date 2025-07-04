// Copyright VUEDK, Inc. All Rights Reserved.

#include "GameEffectsCueManager.h"
#include "Factories/GameEffectsCueFactory.h"

UGameEffectsCue* UGameEffectsCueManager::StartGameEffectsCue(AActor* Instigator, UGameEffectsCueData* GameEffectsCueData)
{
	UGameEffectsCue* Cue = UGameEffectsCueFactory::CreateGameEffectsCue(Instigator, GameEffectsCueData);

	if (!IsValid(Cue))
		return nullptr;

	Cue->StartCue();
	return Cue;
}
