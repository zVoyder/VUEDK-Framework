// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Data/SaveGame/SlotInfoItem.h"
#include "SlotInfoData.generated.h"

USTRUCT(BlueprintType)
struct FSlotInfoData
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FString SlotInfoName;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FDateTime LastSaveDate;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	double TimePlayed;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USlotInfoItem* SlotInfoItem;

	FSlotInfoData(): SlotInfoName(""),
	                 LastSaveDate(FDateTime::Now()),
	                 TimePlayed(0),
	                 SlotInfoItem(nullptr)
	{
	}

	explicit FSlotInfoData(const FString& SlotName): SlotInfoName(SlotName),
	                                                 LastSaveDate(FDateTime::Now()),
	                                                 TimePlayed(0),
	                                                 SlotInfoItem(nullptr)
	{
	}

	bool operator==(const FSlotInfoData& Other) const
	{
		return SlotInfoName == Other.SlotInfoName;
	}

	bool operator<(const FSlotInfoData& Other) const
	{
		return LastSaveDate < Other.LastSaveDate;
	}

	bool operator>(const FSlotInfoData& Other) const
	{
		return LastSaveDate > Other.LastSaveDate;
	}
};
