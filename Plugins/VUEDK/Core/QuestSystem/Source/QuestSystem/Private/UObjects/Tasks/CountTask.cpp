// Copyright VUEDK, Inc. All Rights Reserved.

#include "UObjects/Tasks/CountTask.h"
#include "Data/DataAssets/CountTaskData.h"

void UCountTask::Init(UTaskData* InitData, UQuestBase* Quest)
{
	Super::Init(InitData, Quest);

	CurrentCount = 0;
	if (const UCountTaskData* CountTaskData = Cast<UCountTaskData>(InitData))
		CountToAchieve = CountTaskData->CountToAchieve;
}

void UCountTask::AchieveTask(const bool bFullyAchieve)
{
	if (bIsAchieved) return;

	if (bFullyAchieve)
	{
		CurrentCount = CountToAchieve;
		TriggerAchievement();
	}
	else
	{
		CurrentCount++;
		OnProgressUpdated.Broadcast(CurrentCount);
		if (CurrentCount >= CountToAchieve)
			TriggerAchievement();
	}
}

void UCountTask::ResetTask()
{
	Super::ResetTask();
	CurrentCount = 0;
}

void UCountTask::SetCurrentCount(int32 NewCount)
{
	CurrentCount = FMath::Clamp(NewCount, 0, CountToAchieve);
}

int32 UCountTask::GetCurrentCount() const
{
	return CurrentCount;
}
