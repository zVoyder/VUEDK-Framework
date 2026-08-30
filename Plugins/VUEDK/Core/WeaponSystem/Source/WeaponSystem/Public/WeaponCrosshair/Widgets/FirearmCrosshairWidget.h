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

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ToolTip = "Index of the firearm behaviour to monitor."), Category = "Firearm Crosshair")
	int32 FirearmBehaviourIndex = 0;
	
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

	/**
	 * Called when a reload starts.
	 * @param Request The reload request data.
	 */
	UFUNCTION(BlueprintNativeEvent)
	void OnReloadStart(const FReloadRequest& Request);

	/**
	 * Called when a reload ends.
	 * @param Request The reload request data.
	 * @param bInterrupted Whether the reload was interrupted, if not the reload completed successfully.
	 */
	UFUNCTION(BlueprintNativeEvent)
	void OnReloadEnd(const FReloadRequest& Request, bool bInterrupted);

	/**
	 * Called when ammo is inserted during a reload.
	 * @param Behaviour The shooter behaviour where ammo was inserted.
	 * @param InsertedAmmo The amount of ammo that was inserted.
	 * @param RemainingAmmo The remaining ammo available after insertion.
	 */
	UFUNCTION(BlueprintNativeEvent)
	void OnReloadInsertAmmo(UShooterBehaviourBase* Behaviour, int32 InsertedAmmo, int32 RemainingAmmo);
	
	UFUNCTION(BlueprintCallable)
	void AnimateCrosshair(UWidgetAnimation* CrosshairAnimation);

private:
	bool Check() const;
};
