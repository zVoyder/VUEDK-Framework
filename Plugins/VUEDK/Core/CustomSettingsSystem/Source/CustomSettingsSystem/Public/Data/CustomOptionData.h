// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CustomOptionObject.h"
#include "CustomOptionData.generated.h"

USTRUCT(NotBlueprintType)
struct FCustomOptionData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = Setting)
	float DefaultValue;
	UPROPERTY(EditAnywhere, Category = Setting)
	float MinValue;
	UPROPERTY(EditAnywhere, Category = Setting)
	float MaxValue;
	UPROPERTY(EditAnywhere, Category = Setting, meta = (ToolTip = "Object used to translate this option value into concrete engine or subsystem changes."))
	TSoftClassPtr<UCustomOptionObject> CustomOptionClass;

	FCustomOptionData() : DefaultValue(0.f),
	                      MinValue(0.f),
	                      MaxValue(1.f)
	{
	}
};
