// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Squad/Squad.h"
#include "UObject/Object.h"
#include "SquadInfo.generated.h"

class USquadMember;

USTRUCT(BlueprintType)
struct FSquadInfo
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	USquadMember* Leader;
	UPROPERTY(BlueprintReadOnly)
	FName SquadID;
	UPROPERTY(BlueprintReadOnly)
	TArray<USquadMember*> Members;
	UPROPERTY(BlueprintReadOnly)
	bool bIsLocked;

	FSquadInfo()
		: Leader(nullptr),
		  SquadID(),
		  Members(),
		  bIsLocked(false)
	{
	}

	explicit FSquadInfo(const USquad* Squad)
	{
		if (!IsValid(Squad))
			return;
		
		Leader = Squad->GetLeader();
		SquadID = Squad->GetSquadId();
		Members = Squad->GetMembers();
		bIsLocked = Squad->IsLocked();
	}
};
