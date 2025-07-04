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
	TSubclassOf<UCrosshairWidget> DefaultCrosshairWidgetClass;
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
	void SetDefaultCrosshairInViewport(UObject* Payload = nullptr);

	UFUNCTION(BlueprintCallable)
	void SetCrosshairInViewport(const TSubclassOf<UCrosshairWidget> CrosshairWidgetClass, UObject* Payload = nullptr);

	UFUNCTION(BlueprintCallable)
	void OpenCrosshairInViewport() const;
	
	UFUNCTION(BlueprintCallable)
	void CloseCrosshairInViewport(const bool bRemoveFromViewport = false);

	UFUNCTION(BlueprintCallable)
	void RemoveCrosshairFromViewport();

	UFUNCTION(BlueprintCallable)
	UUserWidget* GetCurrentCrosshairWidget() const;

	UFUNCTION(BlueprintCallable)
	void ShowCrosshair() const;

	UFUNCTION(BlueprintCallable)
	void HideCrosshair() const;
	
	UFUNCTION(BlueprintPure)
	bool IsSwitchingCrosshairs() const;

	UFUNCTION(BlueprintPure)
	bool IsCrosshairVisible() const;

private:
	void CreateAndOpen();
	
	UFUNCTION()
	void OnSwitchCrosshairs();

	UFUNCTION()
	void OnCompleteSwitchCrosshairs();
};
