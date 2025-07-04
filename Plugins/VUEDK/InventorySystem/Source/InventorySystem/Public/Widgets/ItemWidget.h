// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Base/ItemBase.h"
#include "Blueprint/UserWidget.h"
#include "Widgets/BetterDropReceiverWidget.h"
#include "ItemWidget.generated.h"

UCLASS()
class INVENTORYSYSTEM_API UItemWidget : public UBetterDropReceiverWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly)
	UItemBase* Item;

public:
	UFUNCTION(BlueprintCallable)
	void Init(UItemBase* InItem);

	UFUNCTION(BlueprintImplementableEvent)
	void OnInit(UItemBase* InItem);

protected:
	UFUNCTION(BlueprintPure)
	bool Check() const;
};
