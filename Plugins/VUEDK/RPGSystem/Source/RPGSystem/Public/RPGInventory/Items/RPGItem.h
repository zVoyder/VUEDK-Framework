// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "RPGInventory/Data/SaveData/RPGItemSaveData.h"
#include "TetrisInventory/TetrisItem.h"
#include "RPGInventory/RPGInventoriesManager.h"
#include "RPGInventory/RPGInventory.h"
#include "RPGItemsGeneration/Data/Rarities/RPGRarityLevelData.h"
#include "RPGItemsGeneration/Data/Visuals//RPGItemVisualDetails.h"
#include "RPGItem.generated.h"

class URPGItemData;

UCLASS()
class RPGSYSTEM_API URPGItem : public UTetrisItem
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly)
	URPGRarityLevelData* RarityLevel;
	UPROPERTY(BlueprintReadOnly)
	FRPGItemVisualDetails VisualDetails;

public:
	URPGItem();

	virtual void Init(UObject* WorldContextObject, UItemDataBase* Data) override;

	virtual void NativeOnPostGeneration();
	
	FRPGItemSaveData CreateRPGItemSaveData() const;

	void LoadRPGItemSaveData(URPGInventory* LoadingInventory, const FRPGItemSaveData& RPGItemSaveData);

	virtual FText GetItemFullName() const override;

	virtual FSlateBrush GetItemIcon() const override;

	virtual FText GetItemDescription() const override;

	/**
	 * Gets the RPGItemData associated with this item.
	 * @return Pointer to the URPGItemData.
	 */
	UFUNCTION(BlueprintPure)
	URPGItemData* GetRPGItemData() const;

	virtual bool CanStackItem_Implementation(UItemBase* OtherItem) const override;

protected:
	virtual void SetItemMeshToLoad() override;

	/**
	 * Called after the item has been generated.
	 */
	UFUNCTION(BlueprintImplementableEvent)
	void OnPostGeneration();
};
