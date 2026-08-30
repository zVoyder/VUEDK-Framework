// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "RPGItemSaveData.h"
#include "RPGGearItemSaveData.generated.h"

UCLASS()
class RPGSAVEBRIDGE_API URPGGearItemSaveData : public URPGItemSaveData
{
	GENERATED_BODY()

public:
	UPROPERTY(SaveGame, BlueprintReadWrite)
	TMap<FGuid, float> SavedGearStats;

public:
	virtual bool SaveObjectData(UObject* ObjectToSave) override;

	virtual bool LoadObjectData(UObject* ObjectToLoad) override;
};
