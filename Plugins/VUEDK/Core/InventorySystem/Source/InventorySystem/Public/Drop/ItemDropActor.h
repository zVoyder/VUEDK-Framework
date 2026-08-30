// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Base/ItemBase.h"
#include "GameFramework/Actor.h"
#include "ItemDropActor.generated.h"

UCLASS(Blueprintable, BlueprintType)
class INVENTORYSYSTEM_API AItemDropActor : public AActor
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly)
	UItemBase* RelatedItem;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UStaticMeshComponent* DropMesh;
	
public:
	AItemDropActor();

	/**
	 * Initializes the item drop actor with the specified item.
	 * @param Item The item to initialize the drop actor with.
	 */
	void Init(UItemBase* Item);

protected:
	virtual void BeginDestroy() override;

	/**
	 * Called when the actor is initialized.
	 */
	UFUNCTION(BlueprintNativeEvent)
	void OnInit();
	
private:
	/**
	 * Checks if the item is valid.
	 * @return True if the item is valid, false otherwise.
	 */
	bool Check() const;

	UFUNCTION()
	void OnItemMeshLoaded(UStaticMesh* Mesh);
};
