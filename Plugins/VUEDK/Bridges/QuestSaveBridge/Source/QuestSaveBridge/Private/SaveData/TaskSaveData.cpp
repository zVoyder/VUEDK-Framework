// Copyright VUEDK, Inc. All Rights Reserved.

#include "SaveData/TaskSaveData.h"
#include "UObjects/Tasks/CountTask.h"
#include "UObjects/Tasks/TaskBase.h"

bool UTaskSaveData::SaveObjectData(UObject* ObjectToSave)
{
	UTaskBase* Task = Cast<UTaskBase>(ObjectToSave);
	if (!IsValid(Task))
		return false;

	bSavedIsAchieved = Task->bIsAchieved;

	if (const UCountTask* CountTask = Cast<UCountTask>(Task)) // We can use a map of task types to avoid multiple casts if there are many task types, for now it's fine
		SavedAchieveCount = CountTask->GetCurrentCount();
	else
		SavedAchieveCount = 0;
	
	return Super::SaveObjectData(ObjectToSave);
}

bool UTaskSaveData::LoadObjectData(UObject* ObjectToLoad)
{
	UTaskBase* Task = Cast<UTaskBase>(ObjectToLoad);
	if (!IsValid(Task))
		return false;

	Task->bIsAchieved = bSavedIsAchieved;
	
	if (Task->bIsAchieved)
		Task->AchieveTask(true);
	
	if (UCountTask* CountTask = Cast<UCountTask>(Task))
		CountTask->SetCurrentCount(SavedAchieveCount);
	
	return Super::LoadObjectData(ObjectToLoad);
}
