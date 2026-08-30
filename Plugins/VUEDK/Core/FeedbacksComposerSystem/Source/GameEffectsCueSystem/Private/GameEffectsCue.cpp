// Copyright VUEDK, Inc. All Rights Reserved.

#include "GameEffectsCue.h"
#include "GameEffectsCueSystem.h"
#include "GameEffectsSubsystem.h"

void UGameEffectsCue::Init(AActor* InInstigator, UObject* Payload)
{
	if (!IsValid(InInstigator))
	{
		UE_LOG(LogGameEffectsCueSystem, Warning, TEXT("UGameEffectsCue::Init: Invalid Instigator."));
		return;
	}

	Instigator = InInstigator;
	const UWorld* World = GetWorld();
	if (!IsValid(World))
	{
		UE_LOG(LogGameEffectsCueSystem, Warning, TEXT("UGameEffectsCue::Init: Unable to get World from Instigator."));
		return;
	}

	UGameEffectsSubsystem* Subsystem = World->GetSubsystem<UGameEffectsSubsystem>();
	if (!IsValid(Subsystem))
	{
		UE_LOG(LogGameEffectsCueSystem, Warning, TEXT("UGameEffectsCue::Init: GameEffectsSubsystem not found in World."));
		return;
	}

	Subsystem->RegisterCue(this);

	for (UGameEffectBase* Effect : Effects)
	{
		if (!IsValid(Effect))
			continue;

		Effect->Init(Instigator, Payload, this);
	}
}

void UGameEffectsCue::StartCue(const bool bSkipIfPlaying)
{
	StartCueAtIndex(EffectStartIndex, bSkipIfPlaying);
}

void UGameEffectsCue::StartCueAtIndex(const int32 Index, const bool bSkipIfPlaying)
{
	if (bSkipIfPlaying && bIsPlaying)
		return;
	
	EffectStartIndex = Index;
	bIsPlaying = true;
	OnCueStarted.Broadcast();
	PlayEffectAtIndex(EffectStartIndex);
	
	for (UGameEffectBase* Effect : Effects)
	{
		if (!IsValid(Effect))
			continue;
	
		Effect->CueStart();
	}
}

void UGameEffectsCue::StopCue()
{
	bIsPlaying = false;
	OnCueStopped.Broadcast();
	
	if (UGameEffectBase* Effect = Effects.IsValidIndex(CurrentIndex) ? Effects[CurrentIndex] : nullptr)
		Effect->Stop();
	
	for (UGameEffectBase* Effect : Effects)
	{
		if (!IsValid(Effect))
			continue;
	
		Effect->CueStop();
	}
	
	FinishCue();
}

void UGameEffectsCue::PauseCue()
{
	bIsPlaying = false;
	OnCuePaused.Broadcast();
	
	if (UGameEffectBase* Effect = Effects.IsValidIndex(CurrentIndex) ? Effects[CurrentIndex] : nullptr)
		Effect->Pause();

	for (UGameEffectBase* Effect : Effects)	
	{
		if (!IsValid(Effect))
			continue;

		Effect->CuePause();
	}
}

void UGameEffectsCue::ResumeCue()
{
	bIsPlaying = true;
	OnCueResumed.Broadcast();
	NextEffect();

	for (UGameEffectBase* Effect : Effects)
	{
		if (!IsValid(Effect))
			continue;

		Effect->CueResume();
	}
}

void UGameEffectsCue::NextEffect()
{
	PlayEffectAtIndex(++CurrentIndex);
}

void UGameEffectsCue::PreviousEffect()
{
	PlayEffectAtIndex(--CurrentIndex);
}

bool UGameEffectsCue::IsPlaying() const
{
	return bIsPlaying;
}

class UWorld* UGameEffectsCue::GetWorld() const
{
	return IsValid(Instigator) ? Instigator->GetWorld() : nullptr;
}

void UGameEffectsCue::FinishCue()
{
	bIsPlaying = false;
	CurrentIndex = 0;
	OnCueFinished.Broadcast();
	OnCueCompleted.ExecuteIfBound(this);
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
	if (UGameEffectBase* Effect = Effects.IsValidIndex(CurrentIndex) ? Effects[CurrentIndex] : nullptr)
		Effect->Play();
}
