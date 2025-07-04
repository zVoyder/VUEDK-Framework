// Copyright VUEDK, Inc. All Rights Reserved.

#include "GameEffectsCue.h"

void UGameEffectsCue::Init(AActor* Instigator)
{
	for (UGameEffectBase* Effect : Effects)
	{
		if (!IsValid(Effect))
			continue;
		
		Effect->Init(Instigator, this);
	}
}

void UGameEffectsCue::StartCue()
{
	bIsPlaying = true;
	OnCueStarted.Broadcast();
	PlayEffectAtIndex(EffectStartIndex);
}

void UGameEffectsCue::StopCue()
{
	bIsPlaying = false;
	OnCueStopped.Broadcast();
	
	if (UGameEffectBase* Effect = Effects[CurrentIndex])
		Effect->Stop();
}

void UGameEffectsCue::ResumeCue()
{
	bIsPlaying = true;
	OnCueResumed.Broadcast();
	NextEffect();
}

void UGameEffectsCue::InterruptCue()
{
	if (UGameEffectBase* Effect = Effects[CurrentIndex])
		Effect->Interrupt();

	FinishCue();
}

void UGameEffectsCue::NextEffect()
{
	PlayEffectAtIndex(++CurrentIndex);
}

void UGameEffectsCue::PreviousEffect()
{
	PlayEffectAtIndex(--CurrentIndex);
}

void UGameEffectsCue::PlayEffectAtIndex(const int32 Index)
{
	if (!bIsPlaying)
		return;

	if (!Effects.IsValidIndex(Index))
	{
		FinishCue();
		return;
	}

	CurrentIndex = Index;
	if (UGameEffectBase* Effect = Effects[Index])
		Effect->Play();
}

void UGameEffectsCue::FinishCue()
{
	bIsPlaying = false;
	CurrentIndex = 0;
	OnCueFinished.Broadcast();
}
