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
	/**
	 * Initializes the item widget with the specified item.
	 * @param InItem The item to initialize the widget with.
	 */
	UFUNCTION(BlueprintCallable)
	void Init(UItemBase* InItem);

	/**
	 * Blueprint event that is called when the item widget is initialized.
	 * @param InItem The item that has been initialized in the widget.
	 */
	UFUNCTION(BlueprintImplementableEvent)
	void OnInit(UItemBase* InItem);

protected:
	/**
	 * Checks if the item widget is valid and has an item assigned.
	 * @return True if the item widget is valid and has an item, false otherwise.
	 */
	UFUNCTION(BlueprintPure)
	bool Check() const;
};
