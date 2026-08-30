// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "BattlePassRewardData.generated.h"

UCLASS()
class BATTLEPASSSYSTEM_API UBattlePassRewardData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 UnlockLevel = 0;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText RewardName;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (MultiLine = true))
	FText RewardDescription;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FSlateBrush RewardIcon;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FGameplayTag> Tags;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UObject* Payload;

public:
	UBattlePassRewardData() : RewardName(FText::FromString(TEXT("Default Reward Name"))),
	                        RewardDescription(FText::FromString(TEXT("Default Reward Description"))),
	                        Payload(nullptr)
	{
	}
};
