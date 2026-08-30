// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Handlers/ShootModesHandler.h"
#include "Shooter/ShootBarrel.h"
#include "Shooter/ShootPoint.h"
#include "Shooter/Data/ShootData.h"
#include "Shooter/Data/ShootFailReason.h"
#include "Shooter/Behaviours/Handlers/CooldownHandler.h"
#include "Shooter/Behaviours/Handlers/RecoilHandler.h"
#include "Shooter/Behaviours/Handlers/SpreadHandler.h"
#include "Shooter/Managers/Magazine/Magazine.h"
#include "UObject/Object.h"
#include "Weapons/Data/AmmoTypeData.h"
#include "Weapons/Data/WeaponShootData.h"
#include "ShooterBehaviourBase.generated.h"

class UShooter;
enum class EShootType : uint8;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
	FOnBehaviourEnabled,
	UShooterBehaviourBase*, Behaviour
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
	FOnBehaviourDisabled,
	UShooterBehaviourBase*, Behaviour
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(
	FOnBehaviourShootSuccess,
	UShooterBehaviourBase*, Behaviour,
	UShootBarrel*, ShootBarrel,
	int32, ShotIndex
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(
	FOnBehaviourShootFail,
	UShooterBehaviourBase*, Behaviour,
	EShootFailReason, FailReason
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
	FOnEndShootSequence,
	UShooterBehaviourBase*, Behaviour
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(
	FOnBehaviourAmmoChanged,
	UShooterBehaviourBase*, Behaviour,
	UMagazine*, Magazine,
	int32, CurrentAmmo,
	int32, MagSize
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FiveParams(
	FOnBehaviourRefilled,
	UShooterBehaviourBase*, Behaviour,
	UMagazine*, Magazine,
	int32, CurrentAmmo,
	int32, RefilledAmmo,
	int32, RemainingAmmo
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(
	FOnBehaviourFull,
	UShooterBehaviourBase*, Behaviour,
	UMagazine*, Magazine
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(
	FOnBehaviourEmpty,
	UShooterBehaviourBase*, Behaviour,
	UMagazine*, Magazine
);

constexpr int32 HasJustShootTicks = 2;
constexpr float RecoilStrengthMultiplier = 100.f;

UCLASS(Abstract, Blueprintable, BlueprintType, EditInlineNew)
class WEAPONSYSTEM_API UShooterBehaviourBase : public UObject
{
	GENERATED_BODY()

	friend class UShooter;

public:
	UPROPERTY(BlueprintAssignable, Category = Events)
	FOnBehaviourEnabled OnBehaviourEnabled;
	UPROPERTY(BlueprintAssignable, Category = Events)
	FOnBehaviourDisabled OnBehaviourDisabled;
	UPROPERTY(BlueprintAssignable, Category = Events)
	FOnBehaviourShootSuccess OnBehaviourShootSuccess;
	UPROPERTY(BlueprintAssignable, Category = Events)
	FOnBehaviourShootFail OnBehaviourShootFail;
	UPROPERTY(BlueprintAssignable, Category = Events)
	FOnEndShootSequence OnBehaviourShootSequenceEnded;
	UPROPERTY(BlueprintAssignable, Category = Events)
	FOnBehaviourAmmoChanged OnBehaviourAmmoChanged;
	UPROPERTY(BlueprintAssignable, Category = Events)
	FOnBehaviourRefilled OnBehaviourRefilled;
	UPROPERTY(BlueprintAssignable, Category = Events)
	FOnBehaviourFull OnBehaviourFull;
	UPROPERTY(BlueprintAssignable, Category = Events)
	FOnBehaviourEmpty OnBehaviourEmpty;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Shooter")
	bool bUseDirectionToTarget = true;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Shooter")
	UShootData* ShootData;

protected:
	UPROPERTY(BlueprintReadOnly)
	UShooter* Shooter;
	UPROPERTY(BlueprintReadOnly)
	APawn* Owner;
	UPROPERTY(BlueprintReadOnly)
	TArray<UShootPoint*> ShootPoints;

private:
	UPROPERTY()
	UCooldownHandler* CooldownHandler;
	UPROPERTY()
	URecoilHandler* RecoilHandler;
	UPROPERTY()
	USpreadHandler* SpreadHandler;
	UPROPERTY()
	UShootModesHandler* ShootModesHandler;
	UPROPERTY()
	UShootBarrel* ShootBarrel;
	int32 CurrentShootPointIndex;
	bool bIsBehaviourActive;

	// Cached Stats
	float CurrentDamage;
	float CurrentFireRate;
	float CurrentMaxRange;
	float CurrentRecoilStrength;
	float CurrentDefaultSpread;
	bool bCurrentInfiniteAmmo;
	EShootType CurrentShootType;
	int32 CurrentMagazineIndex;

public:
	virtual void Init(UShooter* InShooter);

	UFUNCTION(BlueprintCallable)
	void SetupShootBarrel(UShootBarrel* InShootBarrel);

	void CreateHandlers();

	void SetOwner(APawn* InOwner);

	/**
	 * Enables or disables the shooter behaviour.
	 * @param bEnabled True to enable, false to disable.
	 */
	UFUNCTION(BlueprintCallable)
	void SetBehaviourEnabled(const bool bEnabled);

	/**
	 * Enables the shooter behaviour.
	 */
	UFUNCTION(BlueprintCallable)
	void EnableBehaviour();

	/**
	 * Disables the shooter behaviour.
	 */
	UFUNCTION(BlueprintCallable)
	void DisableBehaviour();

	/**
	 * Requests a shoot action.
	 * @return True if request was successful, false otherwise.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure = false)
	bool Shoot();

	/**
	 * Ends the current shoot sequence.
	 */
	UFUNCTION(BlueprintCallable)
	void EndShootSequence() const;

	/**
	 * Resets the spread of the shooter behaviour.
	 * @param ChangeRate The rate of change for the spread reset.
	 */
	UFUNCTION(BlueprintCallable)
	void ResetSpread(const float ChangeRate = 1.f) const;

	/**
	 * Refills the associated magazine with the specified amount of ammo.
	 * (NOTE: Prefer using the UMagazine versions to avoid duplicate calls when shared across multiple behaviours).
	 * @param Ammo The amount of ammo to add.
	 * @param OutRemainingAmmo The amount of ammo that could not be added (excess).
	 * @return The amount of ammo actually added to the magazine.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure = false)
	int32 RefillMagazine(const int32 Ammo, int32& OutRemainingAmmo);

	/**
	 * Refills the associated magazine to its maximum capacity.
	 * (NOTE: Prefer using the UMagazine versions to avoid duplicate calls when shared across multiple behaviours).
	 */
	UFUNCTION(BlueprintCallable)
	void RefillAllMagazine();

	/**
	 * Refills the associated magazine with a specific type of ammo to check compatibility.
	 * (NOTE: Prefer using the UMagazine versions to avoid duplicate calls when shared across multiple behaviours).
	 * @param AmmoType The type of ammo to use for refilling.
	 * @param Ammo The amount of ammo to add.
	 * @param OutRemainingAmmo The amount of ammo that could not be added (excess).
	 * @return The amount of ammo actually added to the magazine.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure = false)
	int32 RefillWithAmmoType(UAmmoTypeData* AmmoType, const int32 Ammo, int32& OutRemainingAmmo);

	/**
	 * Adds dynamic spread to the shooter behaviour.
	 * @param AddSpread The amount of spread to add.
	 * @param ChangeRate The rate of change for the spread addition.
	 * @param RecoveryRate The rate of recovery from the added spread.
	 */
	UFUNCTION(BlueprintCallable)
	void AddDynamicSpread(const float AddSpread, const float ChangeRate = 1.0f, const float RecoveryRate = 1.0f) const;

	/**
	 * Sets the shoot parameters of the shooter behaviour.
	 * @param NewDamage The new damage value.
	 * @param NewFireRate The new fire rate value.
	 * @param NewRange The new range value.
	 * @param NewMagSize The new magazine size.
	 * @param NewRecoilStrength The new recoil strength.
	 * @param NewDefaultSpread The new default spread value.
	 */
	UFUNCTION(BlueprintCallable)
	void SetShootParams(const float NewDamage, const float NewFireRate, const float NewRange, const int32 NewMagSize, const int32 NewRecoilStrength, const float NewDefaultSpread);

	/**
	 * Sets the damage of the shooter behaviour.
	 * @param NewDamage The new damage value.
	 */
	UFUNCTION(BlueprintCallable)
	void SetDamage(const float NewDamage);

	/**
	 * Sets the fire rate of the shooter behaviour.
	 * @param NewFireRate The new fire rate value.
	 */
	UFUNCTION(BlueprintCallable)
	void SetFireRate(const float NewFireRate);

	/**
	 * Sets the maximum range of the shooter behaviour.
	 * @param NewRange The new maximum range value.
	 */
	UFUNCTION(BlueprintCallable)
	void SetMaxRange(const float NewRange);

	/**
	 * Sets the size of the magazine associated with this shooter behaviour.
	 * @param NewMagSize The new magazine size.
	 */
	UFUNCTION(BlueprintCallable)
	void SetMagSize(const int32 NewMagSize) const;

	/**
	 * Sets the recoil strength of the shooter behaviour.
	 * @param NewRecoilStrength The new recoil strength.
	 */
	UFUNCTION(BlueprintCallable)
	void SetRecoilStrength(const float NewRecoilStrength);

	/**
	 * Instantly sets the spread of the shooter behaviour.
	 * @param InSpread The new spread value.
	 * @param bOverrideDefault Whether to override the default spread.
	 */
	UFUNCTION(BlueprintCallable)
	void InstantSetSpread(const float InSpread, const bool bOverrideDefault = false) const;

	/**
	 * Sets the spread of the shooter behaviour.
	 * @param NewSpread The new spread value.
	 * @param NewSpreadChangeRate The rate of change for the new spread.
	 * @param bOverrideDefault Whether to override the default spread.
	 */
	UFUNCTION(BlueprintCallable)
	void SetSpread(const float NewSpread, const float NewSpreadChangeRate = 1.0f, const bool bOverrideDefault = false) const;

	UFUNCTION(BlueprintCallable)
	void SetDefaultSpread(const float NewDefaultSpread);

	UFUNCTION(BlueprintCallable)
	void SetInfiniteAmmo(const bool bEnabled);

	UFUNCTION(BlueprintCallable)
	void ChangeShootType(const EShootType NewShootType);

	UFUNCTION(BlueprintCallable)
	void ChangeMagazine(const int32 NewMagazineIndex);

	UFUNCTION(BlueprintCallable)
	void ChangeShootMode(const int32 NewShootModeIndex) const;

	UFUNCTION(BlueprintPure)
	bool IsBehaviourActive() const;

	UFUNCTION(BlueprintPure)
	bool IsShooting() const;

	UFUNCTION(BlueprintPure)
	bool IsMagazineEmpty() const;

	UFUNCTION(BlueprintPure)
	bool IsMagazineFull() const;

	UFUNCTION(BlueprintPure)
	bool UsesAmmoOfType(const UAmmoTypeData* InAmmoType) const;

	UFUNCTION(BlueprintPure)
	bool HasInfiniteAmmo() const;

	UFUNCTION(BlueprintPure)
	bool HasEnoughAmmoToShoot() const;

	UFUNCTION(BlueprintPure)
	UShooter* GetShooter() const;
	
	UFUNCTION(BlueprintPure)
	APawn* GetOwner() const;

	/**
	 * Gets the damage of the shooter behaviour.
	 * @return The damage value.
	 */
	UFUNCTION(BlueprintPure)
	float GetDamage() const;

	/**
	 * Gets the fire rate of the shooter (rounds per minute).
	 * @return The fire rate in rounds per minute.
	 */
	UFUNCTION(BlueprintPure)
	float GetFireRate() const;

	/**
	 * Gets the maximum range of the shooter behaviour.
	 * @return The maximum range value.
	 */
	UFUNCTION(BlueprintPure)
	float GetMaxRange() const;

	/**
	 * Gets the size of the magazine associated with this shooter behaviour.
	 * @return The magazine size. -1 if the magazine is invalid.
	 */
	UFUNCTION(BlueprintPure)
	int32 GetMagazineSize() const;

	/**
	 * Gets the current ammo in the magazine associated with this shooter behaviour.
	 * @return The current ammo count. -1 if the magazine is invalid.
	 */
	UFUNCTION(BlueprintPure)
	int32 GetMagazineCurrentAmmo() const;
	
	/**
	 * Gets the recoil strength of the shooter behaviour.
	 * @return The recoil strength.
	 */
	UFUNCTION(BlueprintPure)
	float GetRecoilStrength() const;

	/**
	 * Gets the spread of the shooter behaviour.
	 * @return The spread value.
	 */
	UFUNCTION(BlueprintPure)
	float GetSpread() const;

	UFUNCTION(BlueprintPure)
	float GetDefaultSpread() const;

	UFUNCTION(BlueprintPure)
	bool TryGetReloadMontageIndex(int32& OutIndex) const;

	UFUNCTION(BlueprintPure)
	bool TryGetShootSuccessMontageIndex(int32& OutIndex, const int32 ShotIndex) const;

	UFUNCTION(BlueprintPure)
	bool TryGetShootFailMontageIndex(int32& OutIndex, const EShootFailReason FailReason) const;
	
	/**
	 * Gets the shoot barrel associated with the shooter behaviour.
	 * @return The shoot barrel.
	 */
	UFUNCTION(BlueprintPure)
	UShootBarrel* GetShootBarrel() const;

	/**
	 * Gets the shoot type of the shooter behaviour.
	 * @return The shoot type.
	 */
	UFUNCTION(BlueprintPure)
	EShootType GetShootType() const;

	UFUNCTION(BlueprintPure)
	UMagazine* GetRelatedMagazine() const;

	UFUNCTION(BlueprintPure)
	int32 GetShootModeIndex() const;

	UFUNCTION(BlueprintPure)
	UShootModeBase* GetShootMode() const;

	UFUNCTION(BlueprintPure)
	TEnumAsByte<ECollisionChannel> GetSightTraceChannel() const;

	UFUNCTION(BlueprintPure)
	TSubclassOf<UDamageType> GetDamageTypeClass() const;

	/**
	 * Gets the number of handled shoot requests.
	 * @return The count of handled shoot requests.
	 */
	UFUNCTION(BlueprintPure)
	int32 GetShootHandledRequestsCount() const;

	UFUNCTION(BlueprintPure)
	UCooldownHandler* GetCooldownHandler() const;

	UFUNCTION(BlueprintPure)
	URecoilHandler* GetRecoilHandler() const;

	UFUNCTION(BlueprintPure)
	USpreadHandler* GetSpreadHandler() const;

	UFUNCTION(BlueprintCallable)
	void ResetAll();

	/**
	 * Resets the cooldown of the shooter behaviour.
	 */
	UFUNCTION(BlueprintCallable)
	void ResetCooldown() const;

	UFUNCTION(BlueprintCallable)
	void ResetDamage();

	UFUNCTION(BlueprintCallable)
	void ResetFireRate();

	UFUNCTION(BlueprintCallable)
	void ResetMaxRange();

	UFUNCTION(BlueprintCallable)
	void ResetRecoilStrength();

	UFUNCTION(BlueprintCallable)
	void ResetDefaultSpread();

	UFUNCTION(BlueprintCallable)
	void ResetInfiniteAmmo();

	UFUNCTION(BlueprintCallable)
	void ResetShootType();

	UFUNCTION(BlueprintCallable)
	void ResetMagazine();

	virtual UWorld* GetWorld() const override;

#if WITH_EDITOR
	virtual bool ImplementsGetWorld() const override;
#endif

	/**
     * Deploys the shoot action based on the current shoot type.
     */
	virtual void DeployShootOfType();

protected:
	/**
 	 * Tries to get the camera start, end, and hit points, as well as the rotation.
 	 * @param OutStartPoint Output parameter for the camera start point.
 	 * @param OutEndPoint Output parameter for the camera end point.
 	 * @param OutHitPoint Output parameter for the camera hit point.
 	 * @param OutRotation Output parameter for the camera rotation.
 	 * @param StartPointOffset Optional offset to apply to the start point.
 	 * @return true if the points were successfully retrieved, false otherwise.
 	*/
	UFUNCTION(BlueprintCallable)
	bool TryGetCameraPoints(FVector& OutStartPoint, FVector& OutEndPoint, FVector& OutHitPoint, FRotator& OutRotation, FVector StartPointOffset = FVector::ZeroVector) const;

	/**
	 * Checks if the target point is in line of sight from the start point within a given tolerance.
	 * @param StartPoint The starting point for the line of sight check.
	 * @param TargetPoint The target point to check visibility to.
	 * @param Tolerance The allowed tolerance for the check (default: 50.0f).
	 * @return true if the target is in line of sight, false otherwise.
	 */
	UFUNCTION(BlueprintPure)
	bool IsInLineOfSight(const FVector& StartPoint, const FVector& TargetPoint, const float Tolerance = 50.0f) const;

	/**
	 * Deploys the shoot action to the specified shoot point.
	 * @param ShootPoint The shoot point to deploy the shoot action to.
	 */
	void DeployShoot(UShootPoint* ShootPoint) const;

	/**
	 * Called when the shoot is successful.
	 */
	void ShootSuccess(const int32 RequestIndex);

	/**
	 * Called when the shoot fails.
	 * @param FailReason The reason for the shoot failure.
	 */
	void ShootFail(const EShootFailReason FailReason);

	/**
	 * Called when the shooter behaviour is initialized.
	 */
	UFUNCTION(BlueprintNativeEvent)
	void OnInit();

	/**
	 * Called every tick to update the shooter behaviour.
	 * @param DeltaTime The time elapsed since the last tick.
	 */
	UFUNCTION(BlueprintNativeEvent)
	void OnTickBehaviour(const float DeltaTime);

	/**
	 * Called when the shooter behaviour is enabled.
	 */
	UFUNCTION(BlueprintNativeEvent)
	void OnEnabled();

	/**
	 * Called when the shooter behaviour is disabled.
	 */
	UFUNCTION(BlueprintNativeEvent)
	void OnDisabled();

	/**
	 * Gets the location of the target the shooter is aiming at. By default, it returns the location of the camera hit point.
	 * @return The location of the target the shooter is aiming at.
	 */
	UFUNCTION(BlueprintNativeEvent, meta = (ToolTip = "Get the location of the target the shooter is aiming at. By default, it returns the location of the camera hit point."))
	FVector GetShooterTargetLocation() const;
	
	UFUNCTION(BlueprintNativeEvent)
	int32 GetReloadMontageIndex(const UWeaponShootData* WeaponShootData) const;
	
	UFUNCTION(BlueprintNativeEvent)
	int32 GetShootSuccessMontageIndex(const UWeaponShootData* WeaponShootData, const int32 ShotIndex) const;
	
	UFUNCTION(BlueprintNativeEvent)
	int32 GetShootFailMontageIndex(const UWeaponShootData* WeaponShootData, const EShootFailReason FailReason) const;

	/**
	 * Called when the shoot is deployed.
	 * @param ShootPoint The shoot point used for the shoot.
	 * @param TargetLocation The location of the target.
	 * @param DirectionToTarget The direction to the target, no spread applied.
	 * @param ShootDirection The final shoot direction, with spread applied.
	 */
	UFUNCTION(BlueprintNativeEvent)
	void OnDeployShoot(UShootPoint* ShootPoint, const FVector& TargetLocation, const FVector& DirectionToTarget, const FVector& ShootDirection) const;

	UFUNCTION(BlueprintNativeEvent)
	void OnShootSequenceEnd();

	/**
	 * Called when the shoot is successful.
	 * @param OutShootBarrel The shoot barrel used for the successful shoot.
	 */
	UFUNCTION(BlueprintNativeEvent)
	void OnShootSuccess(const UShootBarrel* OutShootBarrel);

	/**
	 * Called when the shoot fails.
	 * @param FailReason The reason for the shoot failure.
	 */
	UFUNCTION(BlueprintNativeEvent)
	void OnShootFail(const EShootFailReason FailReason);

	/**
	 * Additional condition to check before shooting.
	 * @return Return true if the shoot can proceed, false otherwise.
	 */
	UFUNCTION(BlueprintNativeEvent)
	bool GetShootCondition(UShootBarrel* OutShootBarrel) const;

	/**
	 * Called when the ammo in the magazine changes.
	 * (NOTE: This is a local event, use the UMagazine version to listen to global changes).
	 * @param Magazine The magazine whose ammo has changed.
	 * @param CurrentAmmo The current ammo count in the magazine.
	 * @param MagSize The size of the magazine.
	 */
	UFUNCTION(BlueprintNativeEvent)
	void OnAmmoChange(UMagazine* Magazine, int32 CurrentAmmo, int32 MagSize);

	/**
	 * Called when the magazine is refilled.
	 * (NOTE: This is a local event, use the UMagazine version to listen to global changes).
	 * @param Magazine The magazine that has been refilled.
	 * @param CurrentAmmo The current ammo count in the magazine.
	 * @param RefilledAmmo The amount of ammo that was added to the magazine.
	 * @param RemainingAmmo The amount of ammo that could not be added (excess).
	 */
	UFUNCTION(BlueprintNativeEvent)
	void OnRefill(UMagazine* Magazine, int32 CurrentAmmo, int32 RefilledAmmo, int32 RemainingAmmo);

	/**
	 * Called when the magazine becomes full.
	 * (NOTE: This is a local event, use the UMagazine version to listen to global changes).
	 * @param Magazine The magazine that is full.
	 */
	UFUNCTION(BlueprintNativeEvent)
	void OnFull(UMagazine* Magazine);

	/**
	 * Called when the magazine becomes empty.
	 * (NOTE: This is a local event, use the UMagazine version to listen to global changes).
	 * @param Magazine The magazine that is empty.
	 */
	UFUNCTION(BlueprintNativeEvent)
	void OnEmpty(UMagazine* Magazine);

	virtual bool Check() const;

private:
	/**
	 * Called every tick to update the shooter behaviour.
	 * @param DeltaTime The time elapsed since the last tick.
	 */
	void TickBehaviour(const float DeltaTime);

	/**
	 * Calls the tick handlers of the shooter behaviour.
	 * @param DeltaTime The time elapsed since the last tick.
	 */
	void TickHandlers(const float DeltaTime) const;

	/**
	 * Handles shooting logic for simultaneous shooting modes.
	 */
	void HandleSimultaneousShoot();

	/**
	 * Handles shooting logic for sequential shooting modes.
	 */
	void HandleSequentialShoot();

	/**
	 * Gets the index of the next shoot point.
	 * @return The index of the next shoot point.
	 */
	int32 NextShootPointIndex();

	bool TryConsumeAmmoForShoot();

	void CallEnableEvent();

	void CallDisableEvent();

	UFUNCTION()
	void CallShootSequenceEndEvent();

	void CallShootSuccessEvent(const int32 RequestIndex);

	void CallShootFailEvent(EShootFailReason FailReason);

	UFUNCTION()
	void CallAmmoChangeEvent(const UObject* Instigator, const UMagazine* Magazine, int32 CurrentAmmo, int32 MagSize);

	UFUNCTION()
	void CallRefillEvent(const UObject* Instigator, const UMagazine* Magazine, int32 CurrentAmmo, int32 RefilledAmmo, int32 RemainingAmmo);

	UFUNCTION()
	void CallFullEvent(const UObject* Instigator, const UMagazine* Magazine);

	UFUNCTION()
	void CallEmptyEvent(const UObject* Instigator, const UMagazine* Magazine);

	void BindMagazineEvents(UMagazine* Magazine);

	void UnbindMagazineEvents(UMagazine* Magazine);

	UFUNCTION()
	void OnHandleShootRequest(int32 RequestIndex, UShootModeBase* ShootMode, bool bDeployShoot, bool bSuccess);
};
