// Copyright VUEDK, Inc. All Rights Reserved.

#include "Factories/CrosshairsFactory.h"

UCrosshairWidget* UCrosshairsFactory::CreateCrosshairWidget(const UObject* WorldContextObject, const TSubclassOf<UCrosshairWidget>& CrosshairWidgetClass, UObject* Payload)
{
	if (!IsValid(WorldContextObject) || !IsValid(CrosshairWidgetClass))
		return nullptr;

	UCrosshairWidget* CrosshairWidget = CreateWidget<UCrosshairWidget>(WorldContextObject->GetWorld(), CrosshairWidgetClass);
	CrosshairWidget->Init(Payload);
	CrosshairWidget->AddToViewport(-1);
	return CrosshairWidget;
}
