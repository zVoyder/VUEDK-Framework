// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Base/ItemBase.h"
#include "UObject/Object.h"
#include "EquipPrecondition.generated.h"

UCLASS(Abstract, Blueprintable, BlueprintType, EditInlineNew)
class INVENTORYSYSTEM_API UEquipPrecondition : public UObject
{
	GENERATED_BODY()

public:
	/**
	 * Checks if the precondition is met for equipping an item.
	 * @param Item The item to check the precondition for.
	 * @param Equipment The equipment to check the precondition against.
	 * @return True if the precondition is met, false otherwise.
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintPure)
	bool IsPreconditionMet(UItemBase* Item, UEquipment* Equipment) const;
	
protected:
	virtual class UWorld* GetWorld() const override;

#if WITH_EDITOR
	virtual bool ImplementsGetWorld() const override;
#endif
};
