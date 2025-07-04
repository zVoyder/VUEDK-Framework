// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ItemBase.h"
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
	
	void Init(UItemBase* Item);

protected:
	virtual void BeginDestroy() override;

	UFUNCTION(BlueprintNativeEvent)
	void OnInit();
	
private:
	bool Check() const;

	UFUNCTION()
	void OnItemMeshLoaded(UStaticMesh* Mesh);
};
