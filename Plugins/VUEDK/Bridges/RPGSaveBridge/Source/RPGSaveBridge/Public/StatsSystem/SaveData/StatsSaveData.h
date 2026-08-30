// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Data/SaveData/SaveDataBase.h"
#include "StatsSaveData.generated.h"

UCLASS()
class RPGSAVEBRIDGE_API UStatsSaveData : public USaveDataBase
{
	GENERATED_BODY()

public:
	UPROPERTY(SaveGame, BlueprintReadWrite)
	TMap<FGuid, float> SavedSpecialStats;
	UPROPERTY(SaveGame, BlueprintReadWrite)
	TMap<FGuid, float> SavedCoreStats;

public:
	virtual bool SaveObjectData(UObject* ObjectToSave) override;

	virtual bool LoadObjectData(UObject* ObjectToLoad) override;
};
