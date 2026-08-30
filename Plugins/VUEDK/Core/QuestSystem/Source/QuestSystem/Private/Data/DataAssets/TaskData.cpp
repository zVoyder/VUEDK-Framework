// Copyright VUEDK, Inc. All Rights Reserved.

#include "Data/DataAssets/TaskData.h"
#include "Data/Enums/TaskType.h"

ETaskType UTaskData::GetTaskType() const
{
	return ETaskType::Single;
}
