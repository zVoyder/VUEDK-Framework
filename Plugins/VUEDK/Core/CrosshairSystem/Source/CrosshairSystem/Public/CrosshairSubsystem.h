// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CrosshairSystemSettings.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Widgets/CrosshairWidget.h"
#include "CrosshairSubsystem.generated.h"

UCLASS()
class CROSSHAIRSYSTEM_API UCrosshairSubsystem : public ULocalPlayerSubsystem
{
	GENERATED_BODY()

private:
	UPROPERTY()
	UCrosshairWidget* CurrentCrosshairWidget;
	UPROPERTY()
	TSubclassOf<UCrosshairWidget> CurrentCrosshairWidgetClass;
	UPROPERTY()
	TSubclassOf<UCrosshairWidget> CurrentDefaultCrosshairWidgetClass;
	UPROPERTY()
	TSubclassOf<UCrosshairWidget> GeneralCrosshairWidgetClass;
	UPROPERTY()
	UObject* CurrentCrosshairPayload;
	UPROPERTY()
	const UCrosshairSystemSettings* Settings;
	bool bIsSwitchingCrosshairs = false;

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	
	virtual void Deinitialize() override;

	virtual void PlayerControllerChanged(APlayerController* NewPlayerController) override;

	UFUNCTION(BlueprintCallable)
	void SetGeneralCrosshairInViewport(UObject* Payload = nullptr, bool bOverrideDefault = false);

	/**
	 * @brief Sets the default crosshair widget in the viewport, optionally with a payload.
	 * @param Payload Optional payload to associate with the crosshair widget.
	 */
	UFUNCTION(BlueprintCallable)
	void SetDefaultCrosshairInViewport(UObject* Payload = nullptr);

	/**
	 * @brief Sets the specified crosshair widget in the viewport, optionally with a payload.
	 * @param CrosshairWidgetClass The class of the crosshair widget to set.
	 * @param Payload Optional payload to associate with the crosshair widget, if nullptr, the previous payload will be used.
	 * @param bOverrideDefault If true, overrides the default crosshair widget class with the specified class.
	 */
	UFUNCTION(BlueprintCallable)
	void SetCrosshairInViewport(const TSubclassOf<UCrosshairWidget> CrosshairWidgetClass, UObject* Payload = nullptr, bool bOverrideDefault = false);

	/**
	 * @brief Opens the crosshair widget in the viewport.
	 */
	UFUNCTION(BlueprintCallable)
	void OpenCrosshairInViewport() const;

	/**
	 * @brief Closes the crosshair widget in the viewport.
	 * @param bRemoveFromViewport If true, removes the widget from the viewport after closing.
	 */
	UFUNCTION(BlueprintCallable)
	void CloseCrosshairInViewport(const bool bRemoveFromViewport = false);

	/**
	 * @brief Removes the crosshair widget from the viewport.
	 */
	UFUNCTION(BlueprintCallable)
	void RemoveCrosshairFromViewport();

	/**
	 * @brief Returns the current crosshair widget instance.
	 * @return The current crosshair widget as UUserWidget.
	 */
	UFUNCTION(BlueprintCallable)
	UUserWidget* GetCurrentCrosshairWidget() const;

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
	 * @brief Returns true if a crosshair switch is currently in progress.
	 * @return True if switching crosshairs, false otherwise.
	 */
	UFUNCTION(BlueprintPure)
	bool IsSwitchingCrosshairs() const;

	/**
	 * @brief Returns true if the crosshair widget is currently visible.
	 * @return True if visible, false otherwise.
	 */
	UFUNCTION(BlueprintPure)
	bool IsCrosshairVisible() const;

private:
	/**
	 * @brief Creates and opens the crosshair widget.
	 */
	void CreateAndOpen();
	
	/**
	 * @brief Called when switching crosshairs begins.
	 */
	UFUNCTION()
	void OnSwitchCrosshairs();

	/**
	 * @brief Called when switching crosshairs is completed.
	 */
	UFUNCTION()
	void OnCompleteSwitchCrosshairs();
};
