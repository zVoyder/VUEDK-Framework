// Copyright VUEDK, Inc. All Rights Reserved.

#include "Effects/HitStopFeedback.h"
#include "Kismet/GameplayStatics.h"

void UHitStopFeedback::OnPlay_Implementation()
{
	OriginalTimeDilation = UGameplayStatics::GetGlobalTimeDilation(GetWorld());
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), TimeDilation);
	Finish();
}

void UHitStopFeedback::OnStop_Implementation()
{
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), OriginalTimeDilation);
}

void UHitStopFeedback::OnFinish_Implementation()
{
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), OriginalTimeDilation);
}
