// Copyright VUEDK, Inc. All Rights Reserved.

#include "Agent/Base/GOAPTagObjectBase.h"

void UGOAPTagObjectBase::AssignTag(const FGameplayTag InTag)
{
	Tag = InTag;
}

void UGOAPTagObjectBase::ClearTag()
{
	Tag = FGameplayTag();
}

FGameplayTag UGOAPTagObjectBase::GetTag() const
{
	return Tag;
}
