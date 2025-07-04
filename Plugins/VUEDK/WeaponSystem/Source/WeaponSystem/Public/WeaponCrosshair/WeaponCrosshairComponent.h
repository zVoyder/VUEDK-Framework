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
	
	UFUNCTION(BlueprintNativeEvent)
	void OnWeaponEquipped();

	UFUNCTION(BlueprintNativeEvent)
	void OnWeaponUnequipped();

	bool Check() const;
};
