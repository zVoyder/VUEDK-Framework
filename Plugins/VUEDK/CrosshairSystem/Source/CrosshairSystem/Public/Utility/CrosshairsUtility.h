// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CrosshairSubsystem.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "CrosshairsUtility.generated.h"

UCLASS()
class CROSSHAIRSYSTEM_API UCrosshairsUtility : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

private:
	static UCrosshairSubsystem* CrosshairSubsystem;

public:
	static void Init(UCrosshairSubsystem* InCrosshairSubsystem);

	static void Deinit();
	
	UFUNCTION(BlueprintCallable, Category = "VUEDK|Crosshair|Utility")
	static void SetDefaultCrosshairInViewport(UObject* Payload = nullptr);
	
	UFUNCTION(BlueprintCallable, Category = "VUEDK|Crosshair|Utility")
	static void SetCrosshairInViewport(const TSubclassOf<UCrosshairWidget> CrosshairWidgetClass, UObject* Payload = nullptr);

	UFUNCTION(BlueprintCallable, Category = "VUEDK|Crosshair|Utility")
	static void OpenCrosshairInViewport();

	UFUNCTION(BlueprintCallable, Category = "VUEDK|Crosshair|Utility")
	static void CloseCrosshairInViewport(const bool bRemoveFromViewport = false);

	UFUNCTION(BlueprintCallable, Category = "VUEDK|Crosshair|Utility")
	static void RemoveCrosshairFromViewport();

	UFUNCTION(BlueprintCallable, Category = "VUEDK|Crosshair|Utility")
	static UUserWidget* GetCurrentCrosshairWidget();

	UFUNCTION(BlueprintCallable, Category = "VUEDK|Crosshair|Utility")
	static void ShowCrosshair();

	UFUNCTION(BlueprintCallable, Category = "VUEDK|Crosshair|Utility")
	static void HideCrosshair();

	UFUNCTION(BlueprintPure, Category = "VUEDK|Crosshair|Utility")
	static bool IsSwitchingCrosshairs();

	UFUNCTION(BlueprintPure, Category = "VUEDK|Crosshair|Utility")
	static bool IsCrosshairVisible();

private:
	static bool Check();
};
