// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Base/MemberObjectBase.h"
#include "LeaderController.generated.h"

class USquadMember;

UCLASS(Blueprintable, BlueprintType)
class SQUADSYSTEM_API ULeaderController : public UMemberObjectBase
{
	GENERATED_BODY()

public:
	void ChangeLeadership(bool bIsLeader);
	
	UFUNCTION(BlueprintNativeEvent)
	float EvaluateLeadership(const TArray<USquadMember*>& Members) const;

protected:
	UFUNCTION(BlueprintNativeEvent)
	void OnAssumeLeadership();

	UFUNCTION(BlueprintNativeEvent)
	void OnRemoveLeadership();
};
