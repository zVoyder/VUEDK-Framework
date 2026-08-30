// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Base/MemberObjectBase.h"
#include "MemberEvaluator.generated.h"

class USquadMember;

UCLASS(Blueprintable, BlueprintType)
class SQUADSYSTEM_API UMemberEvaluator : public UMemberObjectBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent)
	float Evaluate(const TArray<USquadMember*>& Members) const;
};
