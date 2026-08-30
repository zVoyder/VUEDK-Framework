// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Data/SaveData/SaveDataBase.h"
#include "Data/SaveData/SaveDataBase.h"
#include "ItemBaseSaveData.generated.h"

class UItemBase;

UCLASS()
class INVENTORYSAVEBRIDGE_API UItemBaseSaveData : public USaveDataBase
{
	GENERATED_BODY()

public:
	UPROPERTY(SaveGame, BlueprintReadOnly)
	FGuid SavedItemDataID;
	UPROPERTY(SaveGame, BlueprintReadOnly)
	int32 SavedQuantity;
	UPROPERTY(SaveGame, BlueprintReadOnly)
	int32 SavedEquipSlotIndex;

public:
	virtual bool SaveObjectData(UObject* ObjectToSave) override;

	virtual bool LoadObjectData(UObject* ObjectToLoad) override;

	/**
	 * Called after LoadObjectData to allow further initialization of the loaded object.
	 * @param ObjectToLoad The object that has been loaded.
	 * @return True if the post-load operations were successful, false otherwise.
	 */
	virtual bool PostLoadObjectDataNative(UObject* ObjectToLoad);

	/**
	 * Called after LoadObjectData to allow further initialization of the loaded object.
	 * @param ObjectToLoad The object that has been loaded.
	 * @return True if the post-load operations were successful, false otherwise.
	 */
	UFUNCTION(BlueprintNativeEvent)
	bool PostLoadObjectData(UObject* ObjectToLoad);
};
