// Copyright VUEDK, Inc. All Rights Reserved.

#include "Effects/Base/GameEffectBase.h"
#include "GameEffectsCueSystem.h"
#include "GameEffectsCue.h"

void UGameEffectBase::Init(AActor* InInstigator, UObject* InPayload, UGameEffectsCue* InRelatedCue)
{
	Instigator = InInstigator;
	Payload = InPayload;
	RelatedCue = InRelatedCue;
	OnInit();
}

void UGameEffectBase::Play()
{
	if (InDelay <= 0.0f)
	{
		SetPlay();
		return;
	}

	ClearTimer();
	const UWorld* World = GetWorld();
	if (!IsValid(World))
		return;
	World->GetTimerManager().SetTimer(DelayTimerHandle, this, &UGameEffectBase::SetPlay, GetInDelay(), false);
}

void UGameEffectBase::Stop()
{
	if (OutDelay <= 0.0f)
	{
		SetStop();
		return;
	}

	ClearTimer();
	const UWorld* World = GetWorld();
	if (!IsValid(World))
		return;
	World->GetTimerManager().SetTimer(DelayTimerHandle, this, &UGameEffectBase::SetStop, GetOutDelay(), false);
}

void UGameEffectBase::Pause()
{
	ClearTimer();
	SetPause();
}

void UGameEffectBase::CueStart()
{
	OnCueStart();
}

void UGameEffectBase::CueStop()
{
	OnCueStop();
}

void UGameEffectBase::CueResume()
{
	OnCueResume();
}

void UGameEffectBase::CuePause()
{
	OnCuePause();
}

void UGameEffectBase::Tick(float DeltaTime)
{
	OnTick(DeltaTime);
}

bool UGameEffectBase::IsTickable() const
{
	return Check() && bIsPlaying;
}

bool UGameEffectBase::IsTickableInEditor() const
{
	return false;
}

TStatId UGameEffectBase::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(UGameEffectBase, STATGROUP_Tickables);
}

#if WITH_EDITOR
bool UGameEffectBase::ImplementsGetWorld() const
{
	return true;
}
#endif

UWorld* UGameEffectBase::GetWorld() const
{
	return IsValid(Instigator) ? Instigator->GetWorld() : nullptr;
}

void UGameEffectBase::FinishExecute()
{
	if (OutDelay <= 0.0f)
	{
		SetFinish();
		return;
	}

	ClearTimer();
	const UWorld* World = GetWorld();
	if (!IsValid(World))
		return;
	World->GetTimerManager().SetTimer(DelayTimerHandle, this, &UGameEffectBase::SetFinish, GetOutDelay(), false);
}

void UGameEffectBase::NextEffect_Implementation()
{
	if (!IsValid(RelatedCue))
	{
		UE_LOG(LogGameEffectsCueSystem, Error, TEXT("UGameEffectBase::Finish: RelatedCue is not valid."));
		return;
	}

	RelatedCue->NextEffect();
}

AActor* UGameEffectBase::GetInstigator() const
{
	return Instigator;
}

UObject* UGameEffectBase::GetPayload() const
{
	return Payload;
}

UGameEffectsCue* UGameEffectBase::GetRelatedCue() const
{
	return RelatedCue;
}

float UGameEffectBase::GetOutDelay() const
{
	return OutDelay;
}

float UGameEffectBase::GetInDelay() const
{
	return InDelay;
}

void UGameEffectBase::OnCueStart_Implementation()
{
}

void UGameEffectBase::OnStop_Implementation()
{
}

void UGameEffectBase::OnInit_Implementation()
{
}

void UGameEffectBase::OnPlay_Implementation()
{
}

void UGameEffectBase::OnFinish_Implementation()
{
}

void UGameEffectBase::OnTick_Implementation(float DeltaTime)
{
}

void UGameEffectBase::OnCueStop_Implementation()
{
}

void UGameEffectBase::OnPause_Implementation()
{
}

void UGameEffectBase::OnCuePause_Implementation()
{
}

void UGameEffectBase::OnCueResume_Implementation()
{
}

void UGameEffectBase::SetPlay()
{
	bIsPlaying = true;
	OnPlay();
}

void UGameEffectBase::SetFinish()
{
	bIsPlaying = false;
	OnFinish();
	NextEffect();
}

void UGameEffectBase::SetStop()
{
	bIsPlaying = false;
	OnStop();
}

void UGameEffectBase::SetPause()
{
	bIsPlaying = false;
	OnPause();
}

void UGameEffectBase::ClearTimer()
{
	const UWorld* World = GetWorld();
	if (!IsValid(World))
		return;
	World->GetTimerManager().ClearTimer(DelayTimerHandle);
	DelayTimerHandle = FTimerHandle();
}

bool UGameEffectBase::Check() const
{
	return IsValid(Instigator) && IsValid(RelatedCue);
}
