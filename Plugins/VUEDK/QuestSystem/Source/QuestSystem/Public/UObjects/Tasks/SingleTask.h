// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObjects/Tasks/TaskBase.h"
#include "UObjects/Quests/QuestBase.h"
#include "SingleTask.generated.h"

UCLASS(NotBlueprintable, BlueprintType)
class QUESTSYSTEM_API USingleTask : public UTaskBase
{
	GENERATED_BODY()

public:
	virtual void AchieveTask(bool bFullyAchieve) override;
};
