// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Agent/GOAPBlackboard.h"
#include "Subsystems/WorldSubsystem.h"
#include "GOAPBlackboardsSubsystem.generated.h"

UCLASS()
class VGOAP_API UGOAPBlackboardsSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

private:
	UPROPERTY()
	TMap<FGameplayTag, TObjectPtr<UGOAPBlackboard>> GlobalBlackboards;

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	
	UGOAPBlackboard* GetGlobalBlackboard(const FGameplayTag& BlackboardTag) const;

private:
	void CreateBlackboards();
};
