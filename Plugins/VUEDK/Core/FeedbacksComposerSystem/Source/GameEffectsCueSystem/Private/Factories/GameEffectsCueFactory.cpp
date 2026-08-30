// Copyright VUEDK, Inc. All Rights Reserved.

#include "Factories/GameEffectsCueFactory.h"
#include "GameEffectsCueSystem.h"

UGameEffectsCue* UGameEffectsCueFactory::CreateGameEffectsCue(AActor* Instigator, UGameEffectsCueData* GameEffectsCueData, UObject* Payload)
{
	if (!IsValid(GameEffectsCueData))
		return nullptr;
	
	if (!IsValid(Instigator))
	{
		UE_LOG(LogGameEffectsCueSystem, Warning, TEXT("UGameEffectsCueFactory::CreateGameEffectsCue: Instigator is not valid."));
		return nullptr;
	}
	
	UGameEffectsCue* Cue = DuplicateObject<UGameEffectsCue>(GameEffectsCueData->GameEffectsCue, Instigator);
	if (!IsValid(Cue))
	{
		UE_LOG(LogGameEffectsCueSystem, Warning, TEXT("UGameEffectsCueFactory::CreateGameEffectsCue: GameEffectsCue is not valid."));
		return nullptr;
	}

	Cue->Effects.Empty();
	for (const auto Effect : GameEffectsCueData->GameEffectsCue->Effects)
	{
		if (!IsValid(Effect))
			continue;

		UGameEffectBase* NewEffect = DuplicateObject<UGameEffectBase>(Effect, Cue);
		if (!IsValid(NewEffect))
			continue;

		Cue->Effects.Add(NewEffect);
	}

	Cue->Init(Instigator, Payload);
	return Cue;
}

UGameEffectsCue* UGameEffectsCueFactory::StartGameEffectsCue(AActor* Instigator, UGameEffectsCueData* GameEffectsCueData, UObject* Payload)
{
	UGameEffectsCue* Cue = CreateGameEffectsCue(Instigator, GameEffectsCueData, Payload);

	if (!IsValid(Cue))
		return nullptr;

	Cue->StartCue();
	return Cue;
}
