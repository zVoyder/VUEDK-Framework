// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "EquipmentSystem/Data/EquipSlotKey.h"
#include "Base/ItemBase.h"
#include "Base/ItemDropActor.h"
#include "EquipmentSystem/EquipPrecondition.h"
#include "EquipmentSystem/Data/EquipSlotKey.h"
#include "EquipmentSystem/Data/PreconditionsMode.h"
#include "Widgets/ItemWidget.h"
#include "ItemDataBase.generated.h"

UCLASS(Abstract, Blueprintable, BlueprintType)
class INVENTORYSYSTEM_API UItemDataBase : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, AdvancedDisplay)
	FGuid ItemDataID;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, AdvancedDisplay)
	FString ItemTypeID;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UItemBase> ItemClass;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AItemDropActor> ItemDropActorClass;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UItemWidget> ItemWidgetClass;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSoftObjectPtr<UStaticMesh> ItemMesh;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText ItemName;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(MultiLine="true"))
	FText ItemDescription;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FSlateBrush ItemIcon;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = "1", UIMin = "1"))
	int32 MaxStackSize;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float ItemWeight;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ToolTip = "Indicates whether the inventory is limited to a single instance of this item type id."))
	bool bIsUnique;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ToolTip = "Indicates whether the item can be consumed."))
	bool bIsConsumable;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (EditCondition = "bIsConsumable", EditConditionHides, ToolTip = "Indicates whether the item will be consumed upon use."))
	bool bConsumeUponUse;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Equipment")
	UEquipSlotKey* EquipSlotKey;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Equipment", meta = (EditCondition = "EquipSlotKey != nullptr"))
	bool bUsePreconditions = false;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Equipment", meta = (EditCondition = "bUsePreconditions && EquipSlotKey != nullptr", ToolTip = "Determines how the preconditions are evaluated when equipping this item."))
	TEnumAsByte<EPreconditionsMode> PreconditionsMode = EPreconditionsMode::All;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Instanced, Category = "Equipment", meta = (EditCondition = "bUsePreconditions && EquipSlotKey != nullptr", ToolTip = "List of preconditions that must be met to equip this item."))
	TArray<UEquipPrecondition*> EquipPreconditions;

public:
	UItemDataBase(): MaxStackSize(1),
	                 ItemWeight(0),
	                 bIsUnique(false),
	                 bIsConsumable(false),
	                 bConsumeUponUse(false),
	                 EquipSlotKey(nullptr)
	{
	}

	FORCEINLINE virtual void PostInitProperties() override
	{
		Super::PostInitProperties();
		ItemTypeID = "Type_" + FGuid::NewGuid().ToString().Left(8);
	}
};
