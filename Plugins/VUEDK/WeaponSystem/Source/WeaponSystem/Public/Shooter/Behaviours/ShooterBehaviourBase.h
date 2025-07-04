// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Shooter/ShootBarrel.h"
#include "Shooter/ShootPoint.h"
#include "Shooter/Data/ShootData.h"
#include "Shooter/Data/ShootFailReason.h"
#include "Shooter/Handlers/CooldownHandler.h"
#include "Shooter/Handlers/RecoilHandler.h"
#include "Shooter/Handlers/SpreadHandler.h"
#include "Shooter/Interfaces/ShooterBehaviour.h"
#include "UObject/Object.h"
#include "ShooterBehaviourBase.generated.h"

class UShooter;
enum class EShootType : uint8;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(
	FOnBehaviourEnabled
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(
	FOnBehaviourDisabled
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(
	FOnBehaviourShootSuccess,
	UShootBarrel*, ShootBarrel,
	int32, ShotIndex
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(
	FOnBehaviourShootFail
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(
	FOnEndShootSequence
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
	FOnBehaviourRefill,
	int32, CurrentAmmo
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(
	FOnCurrentAmmoChanged,
	int32, CurrentAmmo,
	int32, MagSize
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(
	FOnMagEmpty
);

constexpr int32 HasJustShootTicks = 2;
constexpr float RecoilStrengthMultiplier = 100.f;

UCLASS(Abstract, Blueprintable, BlueprintType, EditInlineNew)
class WEAPONSYSTEM_API UShooterBehaviourBase : public UObject, public IShooterBehaviour
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
	FOnEndShootSequence OnEndShootSequence;
	UPROPERTY(BlueprintAssignable, Category = Events)
	FOnBehaviourRefill OnBehaviourRefill;
	UPROPERTY(BlueprintAssignable, Category = Events)
	FOnCurrentAmmoChanged OnCurrentAmmoChanged;
	UPROPERTY(BlueprintAssignable, Category = Events)
	FOnMagEmpty OnMagEmpty;

	// -- Configuration --
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Shooter|Configuration")
	TEnumAsByte<ECollisionChannel> SightTraceChannel = ECollisionChannel::ECC_Visibility;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Shooter|Configuration")
	TSubclassOf<UDamageType> DamageTypeClass = UDamageType::StaticClass();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shooter|Configuration")
	bool bHasInfiniteAmmo = false;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ClampMin = 0, UIMin = 0), Category = "Shooter|Configuration")
	int32 AmmoToConsumePerShot = 1;

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
	UShootBarrel* ShootBarrel;
	FShootData ShootData;
	int32 CurrentAmmo;
	int32 ShotsFired;
	int32 CurrentShootPointIndex;
	int32 HasJustShotTickCount;
	bool bIsBehaviourActive;
	bool bHasJustShot;
	FRotator BaselineControlRotation;

public:
	/**
	 * Initializes the shooter behaviour with the given shooter, shoot data, and shoot barrel.
	 * @param InShooter - The shooter component using this behaviour.
	 * @param InShootData - The shoot data containing configuration for shooting.
	 * @param InShootBarrel - The shoot barrel associated with this behaviour.
	 */
	virtual void Init(UShooter* InShooter, const FShootData InShootData, UShootBarrel* InShootBarrel);

	/**
	 * Creates the necessary handlers for the shooter behaviour.
	 */
	void CreateHandlers();

	/**
	 * Sets the owner of the shooter behaviour.
	 * @param InOwner - The pawn that owns this shooter behaviour.
	 */
	void SetOwner(APawn* InOwner);

	/**
	 * Enables or disables the shooter behaviour.
	 * @param bEnabled - True to enable, false to disable.
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
	 * Executes the shoot action.
	 * @return True if the shoot was successful, false otherwise.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure = false)
	virtual bool Shoot() override;

	/**
	 * Resets the cooldown of the shooter behaviour.
	 */
	UFUNCTION(BlueprintCallable)
	void ResetCooldown() const;

	/**
	 * Ends the current shoot sequence.
	 */
	UFUNCTION(BlueprintCallable)
	void EndShootSequence();

	/**
	 * Resets the spread of the shooter behaviour.
	 * @param ChangeRate - The rate of change for the spread reset.
	 */
	UFUNCTION(BlueprintCallable)
	void ResetSpread(const float ChangeRate = 1.f) const;

	/**
	 * Refills the ammo of the shooter behaviour.
	 * @param Ammo - The amount of ammo to refill.
	 * @return The actual amount of ammo refilled.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure = false)
	virtual int32 Refill(const int32 Ammo) override;

	/**
	 * Refills all magazines of the shooter behaviour.
	 */
	UFUNCTION(BlueprintCallable)
	virtual void RefillAllMagazine() override;

	/**
	 * Adds dynamic spread to the shooter behaviour.
	 * @param AddSpread - The amount of spread to add.
	 * @param ChangeRate - The rate of change for the spread addition.
	 * @param RecoveryRate - The rate of recovery from the added spread.
	 */
	UFUNCTION(BlueprintCallable)
	void AddDynamicSpread(const float AddSpread, const float ChangeRate = 1.0f, const float RecoveryRate = 1.0f) const;

	/**
	 * Sets the shoot parameters of the shooter behaviour.
	 * @param NewDamage - The new damage value.
	 * @param NewFireRate - The new fire rate value.
	 * @param NewRange - The new range value.
	 * @param NewMagSize - The new magazine size.
	 * @param NewRecoilStrength - The new recoil strength.
	 * @param NewDefaultSpread - The new default spread value.
	 */
	UFUNCTION(BlueprintCallable)
	void SetShootParams(const float NewDamage, const float NewFireRate, const float NewRange, const int32 NewMagSize, const int32 NewRecoilStrength, const float NewDefaultSpread);

	/**
	 * Sets the damage of the shooter behaviour.
	 * @param NewDamage - The new damage value.
	 */
	UFUNCTION(BlueprintCallable)
	void SetDamage(const float NewDamage);

	/**
	 * Sets the fire rate of the shooter behaviour.
	 * @param NewFireRate - The new fire rate value.
	 */
	UFUNCTION(BlueprintCallable)
	void SetFireRate(const float NewFireRate);

	/**
	 * Sets the maximum range of the shooter behaviour.
	 * @param NewRange - The new maximum range value.
	 */
	UFUNCTION(BlueprintCallable)
	void SetMaxRange(const float NewRange);

	/**
	 * Sets the magazine size of the shooter behaviour.
	 * @param NewMagSize - The new magazine size.
	 */
	UFUNCTION(BlueprintCallable)
	void SetMagSize(const int32 NewMagSize);

	/**
	 * Sets the recoil strength of the shooter behaviour.
	 * @param NewRecoilStrength - The new recoil strength.
	 */
	UFUNCTION(BlueprintCallable)
	void SetRecoilStrength(float NewRecoilStrength);

	/**
	 * Instantly sets the spread of the shooter behaviour.
	 * @param InSpread - The new spread value.
	 * @param bOverrideDefault - Whether to override the default spread.
	 */
	UFUNCTION(BlueprintCallable)
	void InstantSetSpread(const float InSpread, const bool bOverrideDefault = false) const;

	/**
	 * Sets the spread of the shooter behaviour.
	 * @param NewSpread - The new spread value.
	 * @param NewSpreadChangeRate - The rate of change for the new spread.
	 * @param bOverrideDefault - Whether to override the default spread.
	 */
	UFUNCTION(BlueprintCallable)
	void SetSpread(const float NewSpread, const float NewSpreadChangeRate = 1.0f, const bool bOverrideDefault = false) const;

	/**
	 * Sets the shoot type of the shooter behaviour.
	 * @param NewShootType - The new shoot type.
	 */
	UFUNCTION(BlueprintCallable)
	void SetShootType(const EShootType NewShootType);

	/**
	 * Sets the current ammo of the shooter behaviour.
	 * @param NewAmmo - The new current ammo value.
	 */
	UFUNCTION(BlueprintCallable)
	void SetCurrentAmmo(const int32 NewAmmo);

	/**
	 * Modifies the current ammo of the shooter behaviour.
	 * @param AmmoValue - The value to modify the current ammo by.
	 */
	UFUNCTION(BlueprintCallable)
	void ModifyCurrentAmmo(const int32 AmmoValue);

	/**
	 * Checks if the shooter behaviour is active.
	 * @return True if active, false otherwise.
	 */
	UFUNCTION(BlueprintPure)
	bool IsBehaviourActive() const;

	/**
	 * Gets the owner of the shooter behaviour.
	 * @return The pawn that owns this shooter behaviour.
	 */
	UFUNCTION(BlueprintPure)
	APawn* GetOwner() const;

	/**
	 * Gets the damage of the shooter behaviour.
	 * @return The damage value.
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintPure)
	float GetDamage() const;

	/**
	 * Gets the fire rate of the shooter (rounds per minute).
	 * @return The fire rate in rounds per minute.
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintPure)
	float GetFireRate() const;

	/**
	 * Gets the maximum range of the shooter behaviour.
	 * @return The maximum range value.
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintPure)
	float GetMaxRange() const;

	/**
	 * Gets the magazine size of the shooter behaviour.
	 * @return The magazine size.
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintPure)
	int32 GetMagSize() const;

	/**
	 * Gets the recoil strength of the shooter behaviour.
	 * @return The recoil strength.
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintPure)
	float GetRecoilStrength() const;

	/**
	 * Gets the spread of the shooter behaviour.
	 * @return The spread value.
	 */
	UFUNCTION(BlueprintPure)
	float GetSpread() const;

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

	/**
	 * Gets the number of shots fired by the shooter behaviour.
	 * @return The number of shots fired.
	 */
	UFUNCTION(BlueprintPure)
	int32 GetShotsFired() const;

	/**
	 * Gets the current ammo of the shooter behaviour.
	 * @return The current ammo value.
	 */
	UFUNCTION(BlueprintPure)
	int32 GetCurrentAmmo() const;

	/**
	 * Gets the amount of ammo to consume per shot.
	 * @return The ammo consumption value.
	 */
	UFUNCTION(BlueprintPure)
	int32 GetAmmoToConsume() const;

	/**
	 * Gets the cooldown handler of the shooter behaviour.
	 * @return The cooldown handler.
	 */
	UFUNCTION(BlueprintPure)
	UCooldownHandler* GetCooldownHandler() const;

	/**
	 * Gets the recoil handler of the shooter behaviour.
	 * @return The recoil handler.
	 */
	UFUNCTION(BlueprintPure)
	URecoilHandler* GetRecoilHandler() const;

	/**
	 * Gets the spread handler of the shooter behaviour.
	 * @return The spread handler.
	 */
	UFUNCTION(BlueprintPure)
	USpreadHandler* GetSpreadHandler() const;

	/**
	 * Checks if the shooter has just shot.
	 * @return True if the shooter has just shot, false otherwise.
	 */
	UFUNCTION(BlueprintPure)
	bool HasJustShot() const;

	/**
	 * Gets the world context.
	 * @return The world context.
	 */
	virtual UWorld* GetWorld() const override;

#if WITH_EDITOR
	virtual bool ImplementsGetWorld() const override;
#endif

protected:
	/**
	 * Handles the shoot logic of the shooter behaviour.
	 */
	virtual void HandleShoot();

	/**
	 * Deploys the shoot action to the specified shoot point.
	 * @param ShootPoint - The shoot point to deploy the shoot action to.
	 */
	void DeployShoot(UShootPoint* ShootPoint) const;

	/**
	 * Called when the shoot is successful.
	 */
	void ShootSuccess();

	/**
	 * Called when the shoot fails.
	 * @param FailReason - The reason for the shoot failure.
	 */
	void ShootFail(const EShootFailReason FailReason);

	/**
	 * Called when the shooter behaviour is initialized.
	 */
	UFUNCTION(BlueprintNativeEvent)
	void OnInit();

	/**
	 * Called every tick to update the shooter behaviour.
	 * @param DeltaTime - The time elapsed since the last tick.
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
	 * Called when the shoot is deployed.
	 * @param ShootPoint - The shoot point used for the shoot.
	 * @param TargetLocation - The location of the target.
	 * @param DirectionToTarget - The direction to the target.
	 * @param DirectionToTargetSpreaded - The direction to the target with spread applied.
	 */
	UFUNCTION(BlueprintNativeEvent)
	void OnDeployShoot(UShootPoint* ShootPoint, const FVector& TargetLocation, const FVector& DirectionToTarget, const FVector& DirectionToTargetSpreaded) const;

	/**
	 * Called when the shoot is successful.
	 * @param OutShootBarrel - The shoot barrel used for the successful shoot.
	 */
	UFUNCTION(BlueprintNativeEvent)
	void OnShootSuccess(const UShootBarrel* OutShootBarrel);

	/**
	 * Called when the shoot fails.
	 * @param FailReason - The reason for the shoot failure.
	 */
	UFUNCTION(BlueprintNativeEvent)
	void OnShootFail(const EShootFailReason FailReason);

	/**
	 * Additional condition to check before shooting.
	 * @return True if the condition is met, false otherwise.
	 */
	UFUNCTION(BlueprintNativeEvent)
	bool OnShootCondition(UShootBarrel* OutShootBarrel) const;

	/**
	 * Called when the shooter refills its magazine or ammo.
	 */
	UFUNCTION(BlueprintNativeEvent)
	void OnRefill();

	/**
	 * Called when the magazine is emptied.
	 */
	UFUNCTION(BlueprintNativeEvent)
	void OnMagEmptied();

	/**
	 * Gets the location of the target the shooter is aiming at. By default, it returns the location of the camera hit point.
	 * @return The location of the target the shooter is aiming at.
	 */
	UFUNCTION(BlueprintNativeEvent, meta = (ToolTip = "Get the location of the target the shooter is aiming at. By default, it returns the location of the camera hit point."))
	FVector GetShooterTargetLocation() const;
	
	/**
	 * Tries to get the camera start, end, and hit points, as well as the rotation.
	 * @param OutStartPoint - Output parameter for the camera start point.
	 * @param OutEndPoint - Output parameter for the camera end point.
	 * @param OutHitPoint - Output parameter for the camera hit point.
	 * @param OutRotation - Output parameter for the camera rotation.
	 * @param StartPointOffset - Optional offset to apply to the start point.
	 * @return true if the points were successfully retrieved, false otherwise.
	 */
	UFUNCTION(BlueprintCallable)
	bool TryGetCameraPoints(FVector& OutStartPoint, FVector& OutEndPoint, FVector& OutHitPoint, FRotator& OutRotation, FVector StartPointOffset = FVector::ZeroVector) const;

	/**
	 * Checks if the target point is in line of sight from the start point within a given tolerance.
	 * @param StartPoint - The starting point for the line of sight check.
	 * @param TargetPoint - The target point to check visibility to.
	 * @param Tolerance - The allowed tolerance for the check (default: 50.0f).
	 * @return true if the target is in line of sight, false otherwise.
	 */
	UFUNCTION(BlueprintPure)
	bool IsInLineOfSight(const FVector& StartPoint, const FVector& TargetPoint, const float Tolerance = 50.0f) const;

	/**
	 * Checks if the shooter behaviour is valid and ready for operations.
	 * @return true if valid, false otherwise.
	 */
	virtual bool Check() const;

private:
	/**
	 * Called every tick to update the shooter behaviour.
	 * @param DeltaTime - The time elapsed since the last tick.
	 */
	void TickBehaviour(float DeltaTime);

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

	/**
	 * Tries to consume ammo for a shoot action.
	 * @return true if ammo was consumed, false otherwise.
	 */
	bool TryConsumeAmmoForShoot();

	/**
	 * Checks if there is enough ammo to perform a shoot action.
	 * @return true if there is enough ammo, false otherwise.
	 */
	bool HasEnoughAmmoToShoot() const;

	/**
	 * Checks if the magazine is empty and triggers related logic.
	 */
	void CheckMagEmpty();

	/**
	 * Triggers the logic for when a shot has just been fired.
	 */
	void TriggerHasJustShot();

	/**
	 * Called on the next tick to update the has-just-shot state.
	 */
	UFUNCTION()
	void NextTickHasJustShot();

	/**
	 * Resets the has-just-shot state.
	 */
	UFUNCTION()
	void ResetHasJustShot();
};
