// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Shooter/Data/MagData.h"
#include "UObject/Object.h"
#include "Magazine.generated.h"

class UMagazinesManager;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FiveParams(
	FOnMagazineRefilled,
	const UObject*, Instigator,
	const UMagazine*, Magazine,
	int32, CurrentAmmo,
	int32, RefilledAmmo,
	int32, RemainingAmmo
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(
	FOnMagazineAmmoChanged,
	const UObject*, Instigator,
	const UMagazine*, Magazine,
	int32, CurrentAmmo,
	int32, MagSize
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(
	FOnMagazineFull,
	const UObject*, Instigator,
	const UMagazine*, Magazine
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(
	FOnMagazineEmpty,
	const UObject*, Instigator,
	const UMagazine*, Magazine
);

UCLASS(Blueprintable, BlueprintType, EditInlineNew)
class WEAPONSYSTEM_API UMagazine : public UObject, public FTickableGameObject
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable, Category = Events)
	FOnMagazineRefilled OnMagazineRefilled;
	UPROPERTY(BlueprintAssignable, Category = Events)
	FOnMagazineAmmoChanged OnMagazineAmmoChanged;
	UPROPERTY(BlueprintAssignable, Category = Events)
	FOnMagazineFull OnMagazineFull;
	UPROPERTY(BlueprintAssignable, Category = Events)
	FOnMagazineEmpty OnMagazineEmpty;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FMagData MagazineData;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bInitiallyFull = false;

private:
	UPROPERTY()
	UMagazinesManager* MagazinesManager;
	UPROPERTY()
	TWeakObjectPtr<UObject> PendingInstigator;
	int32 CurrentAmmoCount;
	int32 CurrentSize;
	bool bEmptyPending = false;
	bool bFullPending = false;

public:
	void Init(UMagazinesManager* InMagazinesManager);

	virtual UWorld* GetWorld() const override;
	
	virtual void Tick(float DeltaTime) override;
	
	virtual bool IsTickable() const override;

	virtual TStatId GetStatId() const override;
	
	UFUNCTION(BlueprintCallable)
	void SetCurrentAmmo(const int32 NewAmmo, UObject* Instigator = nullptr);

	UFUNCTION(BlueprintCallable)
	void ModifyCurrentAmmo(const int32 Ammo, UObject* Instigator = nullptr);

	UFUNCTION(BlueprintCallable)
	bool TryConsumeAmmo(const int32 Ammo, UObject* Instigator = nullptr);

	/**
	 * Refills the magazine with the specified amount of ammo.
	 * @param Ammo The amount of ammo to add.
	 * @param OutRemainingAmmo The amount of ammo that could not be added (excess).
	 * @param Instigator The object responsible for the refill action.
	 * @return - The amount of ammo actually added to the magazine.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure = false)
	int32 Refill(const int32 Ammo, int32& OutRemainingAmmo, UObject* Instigator = nullptr);

	/**
	 * Refills the magazine to its maximum capacity.
	 * @param Instigator The object responsible for the refill action.
	 */
	UFUNCTION(BlueprintCallable)
	void RefillAllMagazine(UObject* Instigator = nullptr);

	/**
	 * Refills the magazine with a specific type of ammo.
	 * @param AmmoType The type of ammo to use for refilling.
	 * @param Ammo The amount of ammo to add.
	 * @param OutRemainingAmmo The amount of ammo that could not be added (excess).
	 * @param Instigator The object responsible for the refill action.
	 * @return - The amount of ammo actually added to the magazine.
	 */
	UFUNCTION(BlueprintCallable)
	int32 RefillWithAmmoType(UAmmoTypeData* AmmoType, const int32 Ammo, int32& OutRemainingAmmo, UObject* Instigator = nullptr);
	
	UFUNCTION(BlueprintCallable)
	void SetMagazineSize(int32 NewSize);
	
	UFUNCTION(BlueprintPure)
	int32 GetMagazineSize() const;
	
	UFUNCTION(BlueprintPure)
	UAmmoTypeData* GetAmmoType() const;

	UFUNCTION(BlueprintPure)
	int32 GetCurrentAmmo() const;

	UFUNCTION(BlueprintPure)
	int32 GetNeededAmmoToFull() const;

	UFUNCTION(BlueprintPure)
	bool IsFull() const;

	UFUNCTION(BlueprintPure)
	bool IsEmpty() const;
	
	UFUNCTION(BlueprintPure)
	bool IsOfAmmoType(const UAmmoTypeData* AmmoType) const;

	UFUNCTION(BlueprintPure)
	bool CanConsumeAmmo(const int32 Ammo) const;

	UFUNCTION(BlueprintCallable)
	void ResetMagazineSizeToDefault();

protected:
	UFUNCTION(BlueprintNativeEvent)
	void OnRefill(const UObject* Instigator, int32 CurrentAmmo, int32 RefilledAmmo, int32 RemainingAmmo);

	UFUNCTION(BlueprintNativeEvent)
	void OnAmmoChange(const UObject* Instigator, int32 CurrentAmmo, int32 MagSize);

	UFUNCTION(BlueprintNativeEvent)
	void OnFull(UObject* Instigator);

	UFUNCTION(BlueprintNativeEvent)
	void OnEmpty(UObject* Instigator);

private:
	void CallAmmoChangeEvent(const int32 MagSize, const UObject* Instigator = nullptr);

	void CallRefillEvent(const int32& OutRemainingAmmo, const int32 RefilledAmmo, const UObject* Instigator = nullptr);
	
	void CallMagEmptyEvent(UObject* Instigator = nullptr);
	
	void CallMagFullEvent(UObject* Instigator = nullptr);
};
