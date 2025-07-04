// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "ResourceAttributeData.generated.h"

UCLASS(NotBlueprintable, BlueprintType)
class RESOURCEATTRIBUTESSYSTEM_API UResourceAttributeData : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource Attribute")
	FGameplayTag AttributeTag;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource Attribute")
	float StartingValue = 100.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource Attribute")
	float DefaultMinValue = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource Attribute")
	float DefaultMaxValue = 100.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource Attribute|Regeneration")
	bool bHasRegeneration = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource Attribute|Regeneration", meta = (EditCondition = "bHasRegeneration"))
	bool bRegenOnInit = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource Attribute|Regeneration", meta = (EditCondition = "bHasRegeneration"))
	bool bRegenAfterIncrement = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource Attribute|Regeneration", meta = (EditCondition = "bHasRegeneration"))
	bool bRegenAfterDecrement = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource Attribute|Regeneration", meta = (EditCondition = "bHasRegeneration && bRegenOnInit"))
	float RegenerationDelayOnInit = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource Attribute|Regeneration", meta = (EditCondition = "bHasRegeneration && bRegenAfterIncrement"))
	float RegenerationDelayAfterIncrement = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource Attribute|Regeneration", meta = (EditCondition = "bHasRegeneration && bRegenAfterDecrement"))
	float RegenerationDelayAfterDecrement = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource Attribute|Regeneration", meta = (EditCondition = "bHasRegeneration", ToolTip = "The rate at which the resource regenerates per second."))
	float DefaultRegenerationRate = 10.f;
};
