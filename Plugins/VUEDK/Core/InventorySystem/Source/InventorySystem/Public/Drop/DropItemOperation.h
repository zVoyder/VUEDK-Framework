// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Base/ItemBase.h"
#include "UObject/Object.h"
#include "DropItemOperation.generated.h"

UCLASS(Blueprintable, BlueprintType)
class INVENTORYSYSTEM_API UDropItemOperation : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Around Actor Drop Parameters")
	float MinRangeDrop = 50.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Around Actor Drop Parameters")
	float MaxRangeDrop = 60.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Around Actor Drop Parameters")
	float DropHeight = -50.0f;

private:
	UPROPERTY()
	APlayerController* Instigator;

public:
	void Init(APlayerController* InInstigator);

	UFUNCTION(BlueprintNativeEvent)
	bool DropItem(UItemBase* Item);

	UFUNCTION(BlueprintPure)
	APlayerController* GetInstigator() const;
	
	virtual UWorld* GetWorld() const override;

#if WITH_EDITOR
	virtual bool ImplementsGetWorld() const override;
#endif

protected:
	/**
 	* @brief Tries to drop an item around the specified actor using the set drop parameters.
 	* @param Item The item to drop.
 	* @param Actor The actor to drop the item around.
 	* @param OutItemDropActor Reference to the item drop actor that will be created if the drop is successful.
 	* @return True if the item was dropped successfully, false otherwise.
 	*/
	UFUNCTION(BlueprintCallable, BlueprintPure = false)
	bool TryDropItemAroundActor(UItemBase* Item, AActor* Actor, AItemDropActor*& OutItemDropActor) const;

	/**
	 * @brief Tries to drop an item around the player using the set drop parameters.
	 * @param Item The item to drop.
	 * @param OutItemDropActor Reference to the item drop actor that will be created if the drop is successful.
	 * @return True if the item was dropped successfully, false otherwise.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure = false)
	bool TryDropItemAroundPlayer(UItemBase* Item, AItemDropActor*& OutItemDropActor) const;

private:
	bool Check() const;
};
