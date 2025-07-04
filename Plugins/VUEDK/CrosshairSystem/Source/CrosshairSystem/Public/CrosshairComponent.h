// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Widgets/CrosshairWidget.h"
#include "CrosshairComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class CROSSHAIRSYSTEM_API UCrosshairComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Crosshair")
	TSubclassOf<UCrosshairWidget> CrosshairWidgetClass;

protected:
	UPROPERTY()
	UObject* CrosshairPayload;

public:
	UCrosshairComponent();

	/**
	 * @brief Sets the payload object for the crosshair.
	 * @param Payload The payload object to associate with the crosshair.
	 */
	UFUNCTION(BlueprintCallable)
	void SetPayload(UObject* Payload);

	/**
	 * @brief Sets the default crosshair widget in the viewport.
	 */
	UFUNCTION(BlueprintCallable)
	void SetDefaultCrosshair() const;

	/**
	 * @brief Sets the crosshair widget in the viewport.
	 */
	UFUNCTION(BlueprintCallable)
	void SetCrosshair() const;

	/**
	 * @brief Shows the crosshair widget in the viewport.
	 */
	UFUNCTION(BlueprintCallable)
	void ShowCrosshair() const;

	/**
	 * @brief Hides the crosshair widget in the viewport.
	 */
	UFUNCTION(BlueprintCallable)
	void HideCrosshair() const;

	/**
	 * @brief Closes the crosshair widget in the viewport.
	 */
	UFUNCTION(BlueprintCallable)
	void CloseCrosshairInViewport() const;
};


