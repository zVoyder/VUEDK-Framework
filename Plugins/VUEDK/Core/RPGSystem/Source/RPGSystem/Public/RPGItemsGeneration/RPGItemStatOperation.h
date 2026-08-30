// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "RPGInventory/Items/RPGGearItem.h"
#include "StatsSystem/StatOperation.h"
#include "RPGItemStatOperation.generated.h"

UCLASS(EditInlineNew)
class RPGSYSTEM_API URPGItemStatOperation : public UStatOperation
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float GearBaseValue;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float RarityValue;
	
protected:
	UPROPERTY(BlueprintReadOnly)
	URPGGearItem* RelatedItem;

protected:
	virtual void OnInit_Implementation() override;
};
