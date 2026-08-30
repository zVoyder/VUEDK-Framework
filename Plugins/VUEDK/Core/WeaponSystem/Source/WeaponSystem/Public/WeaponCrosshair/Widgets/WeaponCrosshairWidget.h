// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Weapons/WeaponBase.h"
#include "Widgets/CrosshairWidget.h"
#include "WeaponCrosshairWidget.generated.h"

UCLASS()
class WEAPONSYSTEM_API UWeaponCrosshairWidget : public UCrosshairWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly)
	AWeaponBase* Weapon;
	
protected:
	virtual void NativeDestruct() override;
	
	virtual void OnInit_Implementation() override;

	/**
	 * Called when the weapon attack is successful.
	 */
	UFUNCTION(BlueprintNativeEvent)
	void OnWeaponAttackSuccess();

	/**
	 * Called when the weapon attack fails.
	 */
	UFUNCTION(BlueprintNativeEvent)
	void OnWeaponAttackFailed();
};
