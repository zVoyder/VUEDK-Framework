// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GOAPObjectBase.h"
#include "GOAPTagObjectBase.generated.h"

/**
 * Base class for GOAP agent-related objects that requires an identifying tag.
 */
UCLASS(Abstract)
class VGOAP_API UGOAPTagObjectBase : public UGOAPObjectBase
{
	GENERATED_BODY()

private:
	FGameplayTag Tag;

public:
	void AssignTag(const FGameplayTag InTag);

	void ClearTag();

	UFUNCTION(BlueprintPure)
	FGameplayTag GetTag() const;
};
