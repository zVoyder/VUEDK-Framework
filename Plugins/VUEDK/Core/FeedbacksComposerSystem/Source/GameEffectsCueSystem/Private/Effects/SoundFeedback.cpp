// Copyright VUEDK, Inc. All Rights Reserved.

#include "Effects/SoundFeedback.h"
#include "Kismet/GameplayStatics.h"

void USoundFeedback::OnPlay_Implementation()
{
	if (!IsValid(Sound))
		return;

	if (bIs2D)
	{
		UGameplayStatics::PlaySound2D(GetWorld(), Sound, VolumeMultiplier, PitchMultiplier, StartTime, ConcurrencySettings, GetInstigator(), bIsUISound);
		FinishExecute();
		return;
	}

	if (bAttachToInstigator)
		UGameplayStatics::SpawnSoundAttached(Sound, GetInstigator()->GetRootComponent(), NAME_None, FVector::ZeroVector, EAttachLocation::KeepRelativeOffset, true, VolumeMultiplier, PitchMultiplier, StartTime, AttenuationSettings, ConcurrencySettings, bIsUISound);
	else
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), Sound, GetInstigator()->GetActorLocation(), GetInstigator()->GetActorRotation(), VolumeMultiplier, PitchMultiplier, StartTime, AttenuationSettings, ConcurrencySettings, GetInstigator());

	FinishExecute();
}
