// Copyright VUEDK, Inc. All Rights Reserved.

#include "Widgets/ItemWidget.h"
#include "InventorySystem.h"
#include "Base/InventoryBase.h"

void UItemWidget::Init(UItemBase* InItem)
{
	Item = InItem;

	if (!Check())
	{
		UE_LOG(LogInventorySystem, Warning, TEXT("UItemWidget::Init: Item not valid for widget %s"), *GetName());
		return;
	}

	OnInit(Item);
}

bool UItemWidget::Check() const
{
	return IsValid(Item);
}
