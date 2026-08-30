// Copyright VUEDK, Inc. All Rights Reserved.

#include "Factories/ISFactory.h"
#include "InventorySystem.h"
#include "InventorySystemSettings.h"
#include "Base/ItemBase.h"
#include "Base/Data/ItemDataBase.h"
#include "Drop/ItemDropActor.h"

UItemBase* UISFactory::CreateItem(const UObject* WorldContextObject, UItemDataBase* Data, int32& OutOverflow, const int32 Quantity)
{
	if (!IsValid(Data))
	{
		UE_LOG(LogInventorySystem, Warning, TEXT("UISFactory::CreateIte: Data is nullptr."));
		return nullptr;
	}

	if (!IsValid(WorldContextObject))
	{
		UE_LOG(LogInventorySystem, Warning, TEXT("UISFactory::CreateItem: WorldContextObject is nullptr."));
		return nullptr;
	}

	if (TSubclassOf<UItemBase> ItemClass = Data->ItemClass; ItemClass == nullptr)
	{
		UE_LOG(LogInventorySystem, Warning, TEXT("UISFactory::CreateItem: ItemClass is nullptr. Using UItemBase as default."));
		ItemClass = UItemBase::StaticClass();
	}

	UItemBase* Item = NewObject<UItemBase>(GetTransientPackage(), Data->ItemClass);
	UWorld* World = WorldContextObject->GetWorld();
	if (!IsValid(World))
	{
		UE_LOG(LogInventorySystem, Warning, TEXT("UISFactory::CreateItem: World is nullptr."));
		return nullptr;
	}
	
	Item->Init(World, Data);
	OutOverflow = Item->SetQuantity(Quantity);
	return Item;
}

UItemBase* UISFactory::CreateSingleItem(const UObject* WorldContextObject, UItemDataBase* Data)
{
	int32 Overflow = 0;
	return CreateItem(WorldContextObject, Data, Overflow);
}

AItemDropActor* UISFactory::CreateItemDropActor(const UObject* WorldContextObject, UItemBase* Item)
{
	if (!IsValid(WorldContextObject))
	{
		UE_LOG(LogInventorySystem, Warning, TEXT("UISFactory::CreateItemDropActor: WorldContextObject is nullptr."));
		return nullptr;
	}
	
	if (!IsValid(Item))
	{
		UE_LOG(LogInventorySystem, Warning, TEXT("UISFactory::CreateItemDropActor: Item is nullptr."));
		return nullptr;
	}
	
	UWorld* World = WorldContextObject->GetWorld();
	if (!IsValid(World))
	{
		UE_LOG(LogInventorySystem, Warning, TEXT("UISFactory::CreateItemDropActor: World is nullptr."));
		return nullptr;
	}

	AItemDropActor* ItemDropActor = Cast<AItemDropActor>(World->SpawnActor(Item->GetItemData()->ItemDropActorClass));
	ItemDropActor->Init(Item);
	return ItemDropActor;
}

UDropItemOperation* UISFactory::CreateItemDropOperation(APlayerController* Instigator, const TSubclassOf<UDropItemOperation> DropOperationClass)
{
	if (!IsValid(Instigator))
	{
		UE_LOG(LogInventorySystem, Warning, TEXT("UISFactory::CreateItemDropOperation: PlayerController is not valid."));
		return nullptr;
	}

	if (!IsValid(DropOperationClass))
	{
		UE_LOG(LogInventorySystem, Warning, TEXT("UISFactory::CreateItemDropOperation: DropOperationClass is not valid."));
		return nullptr;
	}

	UDropItemOperation* DropOperation = NewObject<UDropItemOperation>(Instigator, DropOperationClass);
	if (!IsValid(DropOperation))
	{
		UE_LOG(LogInventorySystem, Warning, TEXT("UISFactory::CreateItemDropOperation: DropOperation is not valid."));
		return nullptr;
	}

	DropOperation->Init(Instigator);
	return DropOperation;
}
