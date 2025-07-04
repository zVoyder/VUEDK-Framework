// Copyright VUEDK, Inc. All Rights Reserved.

#include "Effects/GameEffectBase.h"
#include "GameEffectsCueSystem.h"
#include "GameEffectsCue.h"
#include "Kismet/GameplayStatics.h"

void UGameEffectBase::Init(AActor* InInstigator, UGameEffectsCue* InRelatedCue)
{
	Instigator = InInstigator;
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
	GetWorld()->GetTimerManager().SetTimer(DelayTimerHandle, this, &UGameEffectBase::SetPlay, GetInDelay(), false);
}

void UGameEffectBase::Stop()
{
	if (OutDelay <= 0.0f)
	{
		SetStop();
		return;
	}

	ClearTimer();
	GetWorld()->GetTimerManager().SetTimer(DelayTimerHandle, this, &UGameEffectBase::SetStop, GetOutDelay(), false);
}

void UGameEffectBase::Interrupt()
{
	SetStop();
}

void UGameEffectBase::Tick(float DeltaTime)
{
	OnTick(DeltaTime);
}

bool UGameEffectBase::IsTickable() const
{
	return Check() && bIsPlaying;
}

TStatId UGameEffectBase::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(UBetterDragDropOperation, STATGROUP_Tickables);
}

#if WITH_EDITOR
bool UGameEffectBase::ImplementsGetWorld() const
{
	return true;
}
#endif

UWorld* UGameEffectBase::GetWorld() const
{
	return Instigator->GetWorld();
}

void UGameEffectBase::Finish()
{
	if (OutDelay <= 0.0f)
	{
		SetFinish();
		return;
	}

	ClearTimer();
	GetWorld()->GetTimerManager().SetTimer(DelayTimerHandle, this, &UGameEffectBase::SetFinish, GetOutDelay(), false);
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

void UGameEffectBase::ClearTimer()
{
	GetWorld()->GetTimerManager().ClearTimer(DelayTimerHandle);
	DelayTimerHandle = FTimerHandle();
}

bool UGameEffectBase::Check() const
{
	return IsValid(Instigator) && IsValid(RelatedCue);
}
