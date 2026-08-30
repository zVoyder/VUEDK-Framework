// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "HitDetectionSubsystem.h"
#include "Components/ActorComponent.h"
#include "Data/HitBoxBonesData.h"
#include "HitBoxManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_EightParams(
	FOnHitBoxAnyDamage,
	float, BaseDamage,
	float, TotalDamage,
	FVector, HitLocation,
	FVector, HitNormal,
	FName, BoneName,
	const class UDamageType*, DamageType,
	AController*, InstigatedBy,
	AActor*, DamageCauser
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_EightParams(
	FOnHitBoxPointDamage,
	float, BaseDamage,
	float, TotalDamage,
	FVector, HitLocation,
	FVector, HitNormal,
	FName, BoneName,
	const class UDamageType*, DamageType,
	AController*, InstigatedBy,
	AActor*, DamageCauser
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_SixParams(
	FOnHitBoxRadialDamage,
	float, BaseDamage,
	float, TotalDamage,
	const class UDamageType*, DamageType,
	FVector, Origin,
	const FHitResult&, HitInfo,
	AController*, InstigatedBy
);

USTRUCT(NotBlueprintType)
struct FAnyDamageEventData
{
	GENERATED_BODY()
	
	float BaseDamage;
	float TotalDamage;
	FVector HitLocation;
	FVector HitNormal;
	FName BoneName;

	UPROPERTY()
	const UDamageType* DamageType;
	UPROPERTY()
	AController* InstigatedBy;
	UPROPERTY()
	AActor* DamageCauser;

private:
	bool bIsValid;

public:
	FAnyDamageEventData()
		: BaseDamage(0.f),
		  TotalDamage(0.f),
		  HitLocation(FVector::ZeroVector),
		  HitNormal(FVector::ZeroVector),
		  BoneName(NAME_None),
		  DamageType(nullptr),
		  InstigatedBy(nullptr),
		  DamageCauser(nullptr),
		  bIsValid(false)
	{
	}

	FAnyDamageEventData(const float InBaseDamage, const float InTotalDamage, const FVector& InHitLocation, const FVector& InHitNormal, const FName InBoneName, const class UDamageType* InDamageType, AController* InInstigatedBy, AActor* InDamageCauser)
		: BaseDamage(InBaseDamage),
		  TotalDamage(InTotalDamage),
		  HitLocation(InHitLocation),
		  HitNormal(InHitNormal),
		  BoneName(InBoneName),
		  DamageType(InDamageType),
		  InstigatedBy(InInstigatedBy),
		  DamageCauser(InDamageCauser),
		  bIsValid(true)
	{
	}

	FORCEINLINE void Invalidate()
	{
		BaseDamage = 0.f;
		TotalDamage = 0.f;
		HitLocation = FVector::ZeroVector;
		HitNormal = FVector::ZeroVector;
		BoneName = NAME_None;
		DamageType = nullptr;
		InstigatedBy = nullptr;
		DamageCauser = nullptr;
		bIsValid = false;
	}

	FORCEINLINE bool IsValid() const
	{
		return bIsValid;
	}
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class HITDETECTIONSYSTEM_API UHitBoxManager : public UActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable, Category = Events)
	FOnHitBoxAnyDamage OnHitBoxAnyDamage;
	UPROPERTY(BlueprintAssignable, Category = Events)
	FOnHitBoxPointDamage OnHitBoxPointDamage;
	UPROPERTY(BlueprintAssignable, Category = Events)
	FOnHitBoxRadialDamage OnHitBoxRadialDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HitBox")
	bool bUseHitBoxPerBones = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "bUseHitBoxPerBones", EditConditionHides), Category = "HitBox")
	UHitBoxBonesData* HitBoxBonesData;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HitBox")
	bool bNotifyPerformHit = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = 0, UIMin = 0), Category = "HitBox")
	float ReceiveDamageCooldown = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Instanced, Category = "HitBox")
	TArray<UDamageProcessor*> GenericDamageProcessors;

private:
	UPROPERTY()
	UHitDetectionSubsystem* HitDetectionSubsystem;
	UPROPERTY()
	TSet<TWeakObjectPtr<AActor>> RecentlyDamageInstigators;
	FAnyDamageEventData AnyDamageEventData;
	bool bHasRecentlyReceivedDamage = false;
	float Timer = 0.0f;

public:
	UHitBoxManager();

	/**
	 * Checks if the hit box manager uses per-bone hit boxes.
	 * @return True if per-bone hit boxes are used, false otherwise.
	 */
	UFUNCTION(BlueprintPure, Category = "HitBox")
	bool HasHitBoxPerBones() const;

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	virtual void BeginPlay() override;

	/**
	 * Checks if the component can currently receive damage.
	 * @return True if it can receive damage, false otherwise.
	 */
	bool CanReceiveDamage() const;

private:
	float ProcessGenericPointDamage(AActor* DamagedActor, float& Damage, class AController* InstigatedBy, FVector HitLocation, class UPrimitiveComponent* FHitComponent, FName BoneName, FVector ShotFromDirection, const class UDamageType* DamageType, AActor* DamageCauser) const;

	float ProcessGenericRadialDamage(AActor* DamagedActor, float& Damage, const class UDamageType* DamageType, FVector Origin, const FHitResult& HitInfo, class AController* InstigatedBy, AActor* DamageCauser) const;

	float ProcessPointDamage(AActor* DamagedActor, float Damage, class AController* InstigatedBy, FVector HitLocation, class UPrimitiveComponent* FHitComponent, FName BoneName, FVector ShotFromDirection, const class UDamageType* DamageType, AActor* DamageCauser) const;

	float ProcessRadialDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, FVector Origin, const FHitResult& HitInfo, class AController* InstigatedBy, AActor* DamageCauser) const;
	
	/**
	 * Registers all hit zones for this manager.
	 */
	void RegisterHitZones();

	/**
	 * Starts the cooldown period for receiving damage.
	 */
	void StartReceiveDamageCooldown();

	/**
	 * Ends the cooldown period for receiving damage.
	 */
	void EndReceiveDamageCooldown();
	
	void NotifyAnyDamage();

	void NotifyPerformHit(AController* InstigatedBy, AActor* DamagedActor, AActor* DamageCauser, const FVector& HitLocation, const FVector& HitNormal, const FName& BoneName, float DamageMultiplier, float TotalDamage, float BaseDamage) const;

	UFUNCTION()
	void OnTakeAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

	UFUNCTION()
	void OnTakePointDamage(AActor* DamagedActor, float Damage, class AController* InstigatedBy, FVector HitLocation, class UPrimitiveComponent* FHitComponent, FName BoneName, FVector ShotFromDirection, const class UDamageType* DamageType, AActor* DamageCauser);

	UFUNCTION()
	void OnTakeRadialDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, FVector Origin, const FHitResult& HitInfo, class AController* InstigatedBy, AActor* DamageCauser);
	
	UFUNCTION()
	void OnZoneHitPointDamage(float BaseDamage, float TotalDamage, FVector HitLocation, FVector ShotFromDirection, FName BoneName, const class UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser);

	UFUNCTION()
	void OnZoneHitRadialDamage(float BaseDamage, float TotalDamage, const class UDamageType* DamageType, FVector Origin, const FHitResult& HitInfo, AController* InstigatedBy, AActor* DamageCauser);
};
