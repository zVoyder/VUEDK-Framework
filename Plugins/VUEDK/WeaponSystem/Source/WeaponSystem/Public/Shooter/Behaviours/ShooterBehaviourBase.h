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
	virtual void Init(UShooter* InShooter, const FShootData InShootData, UShootBarrel* InShootBarrel);

	void CreateHandlers();

	void SetOwner(APawn* InOwner);

	UFUNCTION(BlueprintCallable)
	void SetBehaviourEnabled(const bool bEnabled);

	UFUNCTION(BlueprintCallable)
	void EnableBehaviour();

	UFUNCTION(BlueprintCallable)
	void DisableBehaviour();

	UFUNCTION(BlueprintCallable, BlueprintPure = false)
	virtual bool Shoot() override;

	UFUNCTION(BlueprintCallable)
	void ResetCooldown() const;

	UFUNCTION(BlueprintCallable)
	void EndShootSequence();

	UFUNCTION(BlueprintCallable)
	void ResetSpread(const float ChangeRate = 1.f) const;

	UFUNCTION(BlueprintCallable, BlueprintPure = false)
	virtual int32 Refill(const int32 Ammo) override;

	UFUNCTION(BlueprintCallable)
	virtual void RefillAllMagazine() override;

	UFUNCTION(BlueprintCallable)
	void AddDynamicSpread(const float AddSpread, const float ChangeRate = 1.0f, const float RecoveryRate = 1.0f) const;

	UFUNCTION(BlueprintCallable)
	void SetShootParams(const float NewDamage, const float NewFireRate, const float NewRange, const int32 NewMagSize, const int32 NewRecoilStrength, const float NewDefaultSpread);

	UFUNCTION(BlueprintCallable)
	void SetDamage(const float NewDamage);

	UFUNCTION(BlueprintCallable)
	void SetFireRate(const float NewFireRate);

	UFUNCTION(BlueprintCallable)
	void SetMaxRange(const float NewRange);

	UFUNCTION(BlueprintCallable)
	void SetMagSize(const int32 NewMagSize);

	UFUNCTION(BlueprintCallable)
	void SetRecoilStrength(float NewRecoilStrength);

	UFUNCTION(BlueprintCallable)
	void InstantSetSpread(const float InSpread, const bool bOverrideDefault = false) const;

	UFUNCTION(BlueprintCallable)
	void SetSpread(const float NewSpread, const float NewSpreadChangeRate = 1.0f, const bool bOverrideDefault = false) const;

	UFUNCTION(BlueprintCallable)
	void SetShootType(const EShootType NewShootType);

	UFUNCTION(BlueprintCallable)
	void SetCurrentAmmo(const int32 NewAmmo);

	UFUNCTION(BlueprintCallable)
	void ModifyCurrentAmmo(const int32 AmmoValue);

	UFUNCTION(BlueprintPure)
	bool IsBehaviourActive() const;

	UFUNCTION(BlueprintPure)
	APawn* GetOwner() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintPure)
	float GetDamage() const;

	/**
	 * Gets the fire rate of the shooter (rounds per minute).
	 * @return The fire rate in rounds per minute.
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintPure)
	float GetFireRate() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintPure)
	float GetMaxRange() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintPure)
	int32 GetMagSize() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintPure)
	float GetRecoilStrength() const;

	UFUNCTION(BlueprintPure)
	float GetSpread() const;

	UFUNCTION(BlueprintPure)
	UShootBarrel* GetShootBarrel() const;

	UFUNCTION(BlueprintPure)
	EShootType GetShootType() const;

	UFUNCTION(BlueprintPure)
	int32 GetShotsFired() const;

	UFUNCTION(BlueprintPure)
	int32 GetCurrentAmmo() const;

	UFUNCTION(BlueprintPure)
	int32 GetAmmoToConsume() const;

	UFUNCTION(BlueprintPure)
	UCooldownHandler* GetCooldownHandler() const;

	UFUNCTION(BlueprintPure)
	URecoilHandler* GetRecoilHandler() const;

	UFUNCTION(BlueprintPure)
	USpreadHandler* GetSpreadHandler() const;

	UFUNCTION(BlueprintPure)
	bool HasJustShot() const;

	virtual UWorld* GetWorld() const override;

#if WITH_EDITOR
	virtual bool ImplementsGetWorld() const override;
#endif

protected:
	virtual void HandleShoot();

	void DeployShoot(UShootPoint* ShootPoint) const;

	void ShootSuccess();

	void ShootFail(const EShootFailReason FailReason);

	UFUNCTION(BlueprintNativeEvent)
	void OnInit();

	UFUNCTION(BlueprintNativeEvent)
	void OnTickBehaviour(const float DeltaTime);

	UFUNCTION(BlueprintNativeEvent)
	void OnEnabled();

	UFUNCTION(BlueprintNativeEvent)
	void OnDisabled();

	UFUNCTION(BlueprintNativeEvent)
	void OnDeployShoot(UShootPoint* ShootPoint, const FVector& TargetLocation, const FVector& DirectionToTarget, const FVector& DirectionToTargetSpreaded) const;

	UFUNCTION(BlueprintNativeEvent)
	void OnShootSuccess(const UShootBarrel* OutShootBarrel);

	UFUNCTION(BlueprintNativeEvent)
	void OnShootFail(const EShootFailReason FailReason);

	/**
	 * Additional condition to check before shooting.
	 * @return True if the condition is met, false otherwise.
	 */
	UFUNCTION(BlueprintNativeEvent)
	bool OnShootCondition(UShootBarrel* OutShootBarrel) const;

	UFUNCTION(BlueprintNativeEvent)
	void OnRefill();

	UFUNCTION(BlueprintNativeEvent)
	void OnMagEmptied();

	/**
	 * Gets the location of the target the shooter is aiming at. By default, it returns the location of the camera hit point.
	 * @return The location of the target the shooter is aiming at.
	 */
	UFUNCTION(BlueprintNativeEvent, meta = (ToolTip = "Get the location of the target the shooter is aiming at. By default, it returns the location of the camera hit point."))
	FVector GetShooterTargetLocation() const;

	UFUNCTION(BlueprintCallable)
	bool TryGetCameraPoints(FVector& OutStartPoint, FVector& OutEndPoint, FVector& OutHitPoint, FRotator& OutRotation, FVector StartPointOffset = FVector::ZeroVector) const;

	UFUNCTION(BlueprintPure)
	bool IsInLineOfSight(const FVector& StartPoint, const FVector& TargetPoint, const float Tolerance = 50.0f) const;

	virtual bool Check() const;

private:
	void TickBehaviour(float DeltaTime);

	void HandleSimultaneousShoot();

	void HandleSequentialShoot();

	int32 NextShootPointIndex();

	bool TryConsumeAmmoForShoot();

	bool HasEnoughAmmoToShoot() const;

	void CheckMagEmpty();

	void TriggerHasJustShot();

	UFUNCTION()
	void NextTickHasJustShot();

	UFUNCTION()
	void ResetHasJustShot();
};
