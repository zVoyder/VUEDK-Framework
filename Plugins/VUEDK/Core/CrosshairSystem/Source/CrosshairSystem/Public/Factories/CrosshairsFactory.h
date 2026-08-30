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
	/**
	 * @brief Creates a crosshair widget of the specified class and optionally associates a payload.
	 * @param WorldContextObject The context object for the world.
	 * @param CrosshairWidgetClass The class of the crosshair widget to create.
	 * @param Payload Optional payload to associate with the widget.
	 * @return The created crosshair widget instance.
	 */
	static UCrosshairWidget* CreateCrosshairWidget(const UObject* WorldContextObject, const TSubclassOf<UCrosshairWidget>& CrosshairWidgetClass, UObject* Payload = nullptr);
};
