// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Weapons/WeaponFirearm.h"
#include "Weapons/Data/ReloadEvent/ReloadEventData.h"
#include "ReloadManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
	FOnReloadStarted,
	const FReloadRequest&, Request
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(
	FOnReloadEnded,
	const FReloadRequest&, Request,
	bool, bInterrupted
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(
	FOnReloadInsertedAmmo,
	UShooterBehaviourBase*, Behaviour,
	int32, InsertedAmmo,
	int32, RemainingAmmo
);

UCLASS(BlueprintType)
class WEAPONSYSTEM_API UReloadManager : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable, Category = Events)
	FOnReloadStarted OnReloadStarted;
	UPROPERTY(BlueprintAssignable, Category = Events)
	FOnReloadEnded OnReloadEnded;
	UPROPERTY(BlueprintAssignable, Category = Events)
	FOnReloadInsertedAmmo OnReloadInsertedAmmo;

private:
	UPROPERTY()
	AWeaponFirearm* WeaponFirearm;
	UPROPERTY()
	UFirearmMontagesManager* FirearmMontageManager;
	FReloadEventData CurrentReloadEventData;
	int32 CurrentReloadIndex = 0;
	bool bIsReloading = false;

public:
	void Init(AWeaponFirearm* InWeaponFirearm);
 
	UFUNCTION(BlueprintCallable)
	void RequestReload(const FReloadRequest& Request);

	UFUNCTION(BlueprintCallable)
	void RequestFullReload(const EReloadMode ReloadMode = EReloadMode::Parallel);

	UFUNCTION(BlueprintCallable)
	void InterruptReload(const float CharacterBlendOutTime = 0.2f, const float WeaponBlendOutTime = 0.2f) const;

	void InsertAmmoInBehaviourOfIndex(const int32 BehaviourIndex);

	void InsertAmmoInBehaviour(UShooterBehaviourBase* Behaviour);
	
	UFUNCTION(BlueprintPure)
	bool IsReloading() const;

	UFUNCTION(BlueprintPure)
	const FReloadEventData& GetReloadEventData() const;

private:
	void StartReloadEvent(const FReloadRequest& Request);

	void EndReloadEvent(const bool bInterrupted);

	bool TryInstantReload(const FReloadRequest& Request, UShooterBehaviourBase* Behaviour);
	
	bool TryStartReloadMontageOfBehaviourIndex(const int32 BehaviourIndex = 0) const;

	int32 FindReloadableBehaviourIndex(const FReloadRequest& Request) const;

	FReloadRequest CreateFullReloadRequest(const EReloadMode ReloadMode) const;

	static FReloadEventData CreateReloadEvent(const FReloadRequest& Request);

	void ListenReloadEndedEvent() const;

	void StopListeningReloadEndedEvent();
	
	void FillRemainingMagazines();

	void OnParallelReloadEnded(const FWeaponMontageData& WeaponMontageData, int32 MontageIndex, bool bInterrupted);

	void OnSequentialReloadEnded(const FWeaponMontageData& WeaponMontageData, int32 MontageIndex, bool bInterrupted);

	UFUNCTION()
	void OnReloadMontageEnded(const FWeaponMontageData& WeaponMontageData, int32 MontageIndex, bool bInterrupted);

	bool Check() const;
};
