// Copyright VUEDK, Inc. All Rights Reserved.

#include "Base/ItemDropActor.h"

AItemDropActor::AItemDropActor(): RelatedItem(nullptr)
{
	PrimaryActorTick.bCanEverTick = true;
	DropMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
}

void AItemDropActor::Init(UItemBase* Item)
{
	RelatedItem = Item;
	
	if (!Check())
		return;
	
	RelatedItem->OnItemMeshLoaded.AddDynamic(this, &AItemDropActor::OnItemMeshLoaded);
	RelatedItem->LoadItemMeshAsync();
	DropMesh->SetCollisionProfileName("OverlapAll");
	OnInit();
}

void AItemDropActor::BeginDestroy()
{
	Super::BeginDestroy();
	
	if (!Check())
		return;
	
	RelatedItem->OnItemMeshLoaded.RemoveDynamic(this, &AItemDropActor::OnItemMeshLoaded);
}

void AItemDropActor::OnInit_Implementation()
{
}

bool AItemDropActor::Check() const
{
	return IsValid(RelatedItem);
}

void AItemDropActor::OnItemMeshLoaded(UStaticMesh* Mesh)
{
	DropMesh->SetStaticMesh(Mesh);
}
