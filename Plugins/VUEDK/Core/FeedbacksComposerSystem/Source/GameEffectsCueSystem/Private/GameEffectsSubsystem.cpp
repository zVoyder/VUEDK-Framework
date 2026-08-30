// Copyright VUEDK, Inc. All Rights Reserved.

#include "GameEffectsSubsystem.h"

void UGameEffectsSubsystem::RegisterCue(UGameEffectsCue* Cue)
{
	ActiveCues.Add(Cue);
	Cue->OnCueCompleted.BindUObject(this, &UGameEffectsSubsystem::OnCueCompleted);
}

void UGameEffectsSubsystem::UnregisterCue(UGameEffectsCue* Cue)
{
	ActiveCues.Remove(Cue);
}

void UGameEffectsSubsystem::OnCueCompleted(UGameEffectsCue* GameEffectsCue)
{
	UnregisterCue(GameEffectsCue);
}
