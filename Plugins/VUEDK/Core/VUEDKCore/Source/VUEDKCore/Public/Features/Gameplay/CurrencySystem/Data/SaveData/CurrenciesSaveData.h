// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Data/SaveData/SaveDataBase.h"
#include "CurrenciesSaveData.generated.h"

UCLASS()
class VUEDKCORE_API UCurrenciesSaveData : public USaveDataBase
{
	GENERATED_BODY()

public:
	UPROPERTY(SaveGame, BlueprintReadWrite)
	TMap<FGuid, int32> SavedCurrencies;

public:
	virtual bool SaveObjectData(UObject* ObjectToSave) override;

	virtual bool LoadObjectData(UObject* ObjectToLoad) override;
};
