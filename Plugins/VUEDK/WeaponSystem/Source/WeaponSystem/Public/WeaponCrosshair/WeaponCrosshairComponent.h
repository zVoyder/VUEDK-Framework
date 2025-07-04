// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CrosshairComponent.h"
#include "Weapons/WeaponBase.h"
#include "WeaponCrosshairComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class WEAPONSYSTEM_API UWeaponCrosshairComponent : public UCrosshairComponent
{
	GENERATED_BODY()

private:
	UPROPERTY()
	AWeaponBase* Weapon;
	
public:
	UWeaponCrosshairComponent();

	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

protected:
	virtual void OnRegister() override;

	/**
	 * Called when the weapon is equipped.
	 */
	UFUNCTION(BlueprintNativeEvent)
	void OnWeaponEquipped();

	/**
	 * Called when the weapon is unequipped.
	 */
	UFUNCTION(BlueprintNativeEvent)
	void OnWeaponUnequipped();

	/**
	 * Checks if the weapon crosshair component is valid and ready to use.
	 * @return True if the component is valid, false otherwise.
	 */
	bool Check() const;
};
