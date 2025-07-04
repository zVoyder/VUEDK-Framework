// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Widgets/CrosshairWidget.h"
#include "CrosshairsFactory.generated.h"

UCLASS()
class CROSSHAIRSYSTEM_API UCrosshairsFactory : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	static UCrosshairWidget* CreateCrosshairWidget(const UObject* WorldContextObject, const TSubclassOf<UCrosshairWidget>& CrosshairWidgetClass, UObject* Payload = nullptr);
};
