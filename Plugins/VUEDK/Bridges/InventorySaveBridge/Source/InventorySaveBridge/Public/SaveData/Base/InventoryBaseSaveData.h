// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ItemBaseSaveData.h"
#include "Data/SaveData/SaveDataBase.h"
#include "InventoryBaseSaveData.generated.h"

class UInventorySaveBehaviour;
class UItemBase;

UCLASS()
class INVENTORYSAVEBRIDGE_API UInventoryBaseSaveData : public USaveDataBase
{
	GENERATED_BODY()

public:
	UPROPERTY(SaveGame, BlueprintReadOnly)
	TArray<UItemBaseSaveData*> SavedItemsData;
	UPROPERTY(SaveGame, BlueprintReadOnly)
	double SavedMaxWeight;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TMap<TSubclassOf<UItemBase>, TSubclassOf<UItemBaseSaveData>> ItemToSaveDataMap;
	UPROPERTY()
	UInventorySaveBehaviour* InventorySaveBehaviour;

public:
	void Init(UInventorySaveBehaviour* InInventorySaveBehaviour);
	
	virtual bool SaveObjectData(UObject* ObjectToSave) override;

	virtual bool LoadObjectData(UObject* ObjectToLoad) override;
	
	virtual void RegisterItemsNative();

	UFUNCTION(BlueprintImplementableEvent)
	void RegisterItems();
	
	UItemBaseSaveData* CreateItemSaveData(const UItemBase* Item);

	UFUNCTION(BlueprintCallable)
	void RegisterItemSaveData(TSubclassOf<UItemBase> ItemClass, TSubclassOf<UItemBaseSaveData> ItemSaveDataClass);
	
	void SaveItem(UItemBase* Item, UItemBaseSaveData* ItemSaveData);

	void PostSaveItem(UItemBase* Item, UItemBaseSaveData* ItemSaveData);
	
	void LoadItem(UItemBase* Item, UItemBaseSaveData* ItemSaveData);

	void PostLoadItem(UItemBase* Item, UItemBaseSaveData* ItemSaveData);

protected:
	virtual void SaveItemNative(UItemBase* Item, UItemBaseSaveData* ItemSaveData);

	virtual void PostSaveItemNative(UItemBase* Item, UItemBaseSaveData* ItemSaveData);

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "SaveItem"))
	void ReceiveSaveItem(UItemBase* Item, UItemBaseSaveData* ItemSaveData);

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "PostSaveItem"))
	void ReceivePostSaveItem(UItemBase* Item, UItemBaseSaveData* ItemSaveData);
	
	virtual void LoadItemNative(UItemBase* Item, UItemBaseSaveData* ItemSaveData);

	virtual void PostLoadItemNative(UItemBase* Item, UItemBaseSaveData* ItemSaveData);
	
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "LoadItem"))
	void ReceiveLoadItem(UItemBase* Item, UItemBaseSaveData* ItemSaveData);

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "PostLoadItem"))
	void ReceivePostLoadItem(UItemBase* Item, UItemBaseSaveData* ItemSaveData);

	bool Check() const;
};
