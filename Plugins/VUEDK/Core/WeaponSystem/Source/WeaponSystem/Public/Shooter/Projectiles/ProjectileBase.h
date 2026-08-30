// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "PooledActorBase.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "ProjectileBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
	FProjectileHitDelegate,
	const FHitResult&, ImpactResult
);

/**
 * AProjectileBase serves as the base class for all projectile types in the weapon system.
 * It provides common functionality such as movement, damage handling, and lifespan management.
 * This class is designed to be extended by specific projectile implementations (e.g., bullets, rockets) that can override its behavior as needed.
 * By default, this class won't dispose the projectile on hit, allowing for projectiles that can bounce or penetrate targets.
 */
UCLASS(Abstract, Blueprintable, BlueprintType)
class WEAPONSYSTEM_API AProjectileBase : public APooledActorBase
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable, Category = Events)
	FProjectileHitDelegate OnProjectileHitEvent;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Projectile", meta = (EditCondition = "bUseSpeedCurve"))
	UCurveFloat* SpeedMultiplierCurve;
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UCapsuleComponent* ProjectileCollision;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UProjectileMovementComponent* ProjectileMovementComponent;
	UPROPERTY()
	AActor* ProjectileInstigator;
	UPROPERTY()
	TSubclassOf<UDamageType> DamageTypeClass;

private:
	UPROPERTY()
	bool bUseSpeedCurve;
	FVector Direction;
	float CurveArea;
	float InitialSpeed;
	float CurrentSpeed;
	float Damage;
	float Range;
	float LifeSpan;
	float CurrentLifeSpan;
	bool bIsProjectileAlive;

public:
	AProjectileBase();

	virtual void Tick(float DeltaSeconds) override;

	/**
	 * Initializes the projectile with the given instigator, damage type, damage, range, speed, and direction.
	 * @param InInstigator The actor that instigated the projectile.
	 * @param InDamageTypeClass The damage type class for the projectile.
	 * @param InDamage The amount of damage the projectile deals.
	 * @param InRange The range of the projectile.
	 * @param InInitialSpeed The initial speed of the projectile.
	 * @param InDirection The direction vector of the projectile.
	 */
	void Init(AActor* InInstigator, const TSubclassOf<UDamageType>& InDamageTypeClass, float InDamage, float InRange, float InInitialSpeed, const FVector& InDirection);

	/**
	 * Called when the pooled actor ends play (object pool integration).
	 */
	virtual void OnPooledActorEndPlay_Implementation() override;
	
	/**
	 * Sets the damage value for the projectile.
	 * @param InDamage The damage value to set.
	 */
	UFUNCTION(BlueprintCallable)
	void SetDamage(float InDamage);

	/**
	 * Sets the damage type class for the projectile.
	 * @param InDamageTypeClass The damage type class to set.
	 */
	UFUNCTION(BlueprintCallable)
	void SetDamageType(const TSubclassOf<UDamageType> InDamageTypeClass);
	
	/**
	 * Sets the range value for the projectile.
	 * @param InRange The range value to set.
	 */
	UFUNCTION(BlueprintCallable)
	void SetRange(const float InRange);

	/**
	 * Sets the direction of the projectile and updates its velocity accordingly.
	 * @param InDirection The direction vector to set.
	 */
	UFUNCTION(BlueprintCallable)
	void SetDirection(const FVector InDirection);

	/**
	 * Sets the speed of the projectile and updates its velocity accordingly.
	 * @param NewSpeed The speed value to set.
	 */
	UFUNCTION(BlueprintCallable)
	void SetSpeed(const float NewSpeed);
	
	/**
	 * Sets the velocity of the projectile.
	 * @param NewVelocity The velocity vector to set.
	 */
	UFUNCTION(BlueprintCallable)
	void SetVelocity(const FVector NewVelocity) const;

	/**
	 * Sets the instigator actor for the projectile.
	 * @param InInstigator The instigator actor to set.
	 */
	UFUNCTION(BlueprintCallable)
	void SetProjectileInstigator(AActor* InInstigator);

	/**
	 * Gets the damage value of the projectile.
	 * @return The damage value as a float.
	 */
	UFUNCTION(BlueprintPure)
	float GetDamage() const;

	/**
	 * Gets the range value of the projectile.
	 * @return The range value as a float.
	 */
	UFUNCTION(BlueprintPure)
	float GetRange() const;

	/**
	 * Gets the velocity of the projectile.
	 * @return The velocity vector of the projectile.
	 */
	UFUNCTION(BlueprintPure)
	FVector GetProjectileVelocity() const;

	/**
	 * Gets the instigator actor of the projectile.
	 * @return Pointer to the instigator actor.
	 */
	UFUNCTION(BlueprintPure)
	AActor* GetProjectileInstigator() const;

	/**
	 * Disposes the projectile, cleaning up and returning it to the pool.
	 */
	UFUNCTION(BlueprintCallable)
	void DisposeProjectile();

	/**
	 * Clears the pooled actor (object pool integration).
	 */
	virtual void ClearPooledActor_Implementation() override;

protected:
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
	/**
	 * Called when the projectile is initialized.
	 */
	UFUNCTION(BlueprintNativeEvent)
	void OnInit();

	virtual void OnProjectileHit(const FHitResult& ImpactResult, const FVector& ImpactVelocity);

	/**
	 * Called when the projectile hits something.
	 * @param ImpactResult The result of the hit, containing information about the impact.
	 * @param ImpactVelocity The velocity of the projectile at the time of impact.
	 */
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "OnProjectileHit"))
	void ReceiveOnProjectileHit(const FHitResult& ImpactResult, const FVector& ImpactVelocity);

	/**
	 * Called when the projectile's lifespan ends.
	 */
	UFUNCTION(BlueprintNativeEvent)
	void OnProjectileLifeSpanEnd();

private:
	float CalculateLifeSpan(const float InRange, const float InSpeed) const;

	void CalculateCurveArea();

	void UpdateProjectileMovementComponent() const;
	
	void UpdateProjectileSpeed() const;

	void UpdateLifeSpan();
	
	void StartProjectileLifeSpan();
	
	void ProcessProjectileLifeSpan(const float DeltaSeconds);
	
	UFUNCTION()
	void EndProjectileLifeSpan();
	
	UFUNCTION()
	void NotifyProjectileHit(const FHitResult& ImpactResult, const FVector& ImpactVelocity);
};
