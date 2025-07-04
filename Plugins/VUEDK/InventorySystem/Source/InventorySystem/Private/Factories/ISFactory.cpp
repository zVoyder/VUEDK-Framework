// Copyright VUEDK, Inc. All Rights Reserved.

#include "Factories/ISFactory.h"
#include "Base/InventoryBase.h"
#include "Base/ItemBase.h"
#include "Base/Data/ItemDataBase.h"
#include "Base/ItemDropActor.h"

UItemBase* UISFactory::CreateItem(UObject* WorldContextObject, UItemDataBase* Data)
{
	if (!IsValid(Data))
	{
		UE_LOG(LogInventorySystem, Error, TEXT("Data in CreateItem() is nullptr."));
		return nullptr;
	}

	if (!IsValid(WorldContextObject))
	{
		UE_LOG(LogInventorySystem, Error, TEXT("WorldContextObject in CreateItem() is nullptr."));
		return nullptr;
	}

	if (TSubclassOf<UItemBase> ItemClass = Data->ItemClass; ItemClass == nullptr)
	{
		UE_LOG(LogInventorySystem, Error, TEXT("Item Class in CreateItem() is nullptr, using default ItemBase class."));
		ItemClass = UItemBase::StaticClass();
	}

	UItemBase* Item = NewObject<UItemBase>(GetTransientPackage(), Data->ItemClass);
	Item->Init(WorldContextObject, Data);
	return Item;
}

UItemBase* UISFactory::CreateItems(UObject* WorldContextObject, UItemDataBase* Data, int32& OutOverflow, const int32 Quantity)
{
	UItemBase* Item = CreateItem(WorldContextObject, Data);

	if (!IsValid(Item))
	{
		UE_LOG(LogInventorySystem, Error, TEXT("Item in CreateItemInQuantity() is nullptr."));
		return nullptr;
	}

	OutOverflow = Item->SetQuantity(Quantity);
	return Item;
}

AItemDropActor* UISFactory::CreateItemDropActor(const UObject* WorldContextObject, UItemBase* Item)
{
	if (!IsValid(WorldContextObject))
	{
		UE_LOG(LogInventorySystem, Error, TEXT("WorldContextObject in CreateItemDropActor() is nullptr."));
		return nullptr;
	}
	
	if (!IsValid(Item))
	{
		UE_LOG(LogInventorySystem, Error, TEXT("Item in CreateItemDropActor() is nullptr."));
		return nullptr;
	}
	
	UWorld* World = WorldContextObject->GetWorld();
	if (!IsValid(World))
	{
		UE_LOG(LogInventorySystem, Error, TEXT("World in CreateItemDropActor() is not valid."));
		return nullptr;
	}

	AItemDropActor* ItemDropActor = Cast<AItemDropActor>(World->SpawnActor(Item->GetItemData()->ItemDropActorClass));
	ItemDropActor->Init(Item);
	return ItemDropActor;
}
