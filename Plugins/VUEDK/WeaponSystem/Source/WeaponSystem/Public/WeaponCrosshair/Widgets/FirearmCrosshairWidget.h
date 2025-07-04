// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "WeaponCrosshairWidget.h"
#include "Weapons/WeaponFirearm.h"
#include "FirearmCrosshairWidget.generated.h"

UCLASS()
class WEAPONSYSTEM_API UFirearmCrosshairWidget : public UWeaponCrosshairWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly)
	AWeaponFirearm* Firearm;

protected:
	virtual void NativeDestruct() override;
	
	virtual void OnInit_Implementation() override;

	UFUNCTION(BlueprintNativeEvent)
	void OnAimEnabled();

	UFUNCTION(BlueprintNativeEvent)
	void OnAimDisabled();

	UFUNCTION(BlueprintNativeEvent)
	void OnSpreadChanged(float Spread);

	UFUNCTION(BlueprintCallable)
	void AnimateCrosshair(UWidgetAnimation* CrosshairAnimation);

private:
	bool Check() const;
};
