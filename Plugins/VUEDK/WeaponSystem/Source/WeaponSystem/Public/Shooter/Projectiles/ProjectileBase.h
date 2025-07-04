// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "PooledActorBase.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "ProjectileBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
	FProjectileHitDelegate,
	const FHitResult&, ImpactResult
);

UCLASS(Abstract, Blueprintable, BlueprintType)
class WEAPONSYSTEM_API AProjectileBase : public APooledActorBase
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintAssignable, Category = Events)
	FProjectileHitDelegate OnProjectileHitEvent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UStaticMeshComponent* MeshComponent;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UProjectileMovementComponent* ProjectileMovementComponent;
	UPROPERTY()
	AActor* ProjectileInstigator;
	UPROPERTY()
	TSubclassOf<UDamageType> DamageTypeClass;
	
private:
	float Damage;
	float Range;
	float RemainingLifeSpan;
	bool bIsProjectileAlive;

public:
	AProjectileBase();

	virtual void Tick(float DeltaSeconds) override;
	
	/**
	 * Initializes the projectile with the given instigator, damage type, damage, range, speed, and direction.
	 * @param InInstigator - The actor that instigated the projectile.
	 * @param InDamageTypeClass - The damage type class for the projectile.
	 * @param InDamage - The amount of damage the projectile deals.
	 * @param InRange - The range of the projectile.
	 * @param InSpeed - The speed of the projectile.
	 * @param InDirection - The direction vector of the projectile.
	 */
	void Init(AActor* InInstigator, const TSubclassOf<UDamageType>& InDamageTypeClass, float InDamage, float InRange, float InSpeed, const FVector& InDirection);

	/**
	 * Called when the pooled actor ends play (object pool integration).
	 */
	virtual void OnPooledActorEndPlay_Implementation() override;
	
	/**
	 * Initializes the velocity and lifespan of the projectile.
	 * @param InRange - The range of the projectile.
	 * @param InSpeed - The speed of the projectile.
	 * @param InDirection - The direction vector of the projectile.
	 */
	UFUNCTION(BlueprintNativeEvent)
	void InitVelocityAndLifeSpan(float InRange, float InSpeed, const FVector& InDirection);

	/**
	 * Calculates the lifespan of the projectile based on range and speed.
	 * @param InRange - The range of the projectile.
	 * @param InSpeed - The speed of the projectile.
	 * @return The calculated lifespan as a float.
	 */
	UFUNCTION(BlueprintNativeEvent)
	float CalculateLifeSpan(const float InRange, const float InSpeed) const;
	
	/**
	 * Sets the damage value for the projectile.
	 * @param InDamage - The damage value to set.
	 */
	UFUNCTION(BlueprintCallable)
	void SetDamage(float InDamage);

	/**
	 * Sets the range value for the projectile.
	 * @param InRange - The range value to set.
	 */
	UFUNCTION(BlueprintCallable)
	void SetRange(float InRange);

	/**
	 * Sets the velocity of the projectile.
	 * @param NewVelocity - The velocity vector to set.
	 */
	UFUNCTION(BlueprintCallable)
	void SetVelocity(const FVector NewVelocity) const;

	/**
	 * Sets the instigator actor for the projectile.
	 * @param InInstigator - The instigator actor to set.
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
	 * Starts the projectile's lifespan countdown.
	 * @param InLifeSpan - The lifespan of the projectile in seconds.
	 */
	void StartProjectileLifeSpan(const float InLifeSpan);

	/**
	 * Called when the projectile is initialized.
	 */
	UFUNCTION(BlueprintNativeEvent)
	void OnInit();

	/**
	 * Called when the projectile hits something.
	 * @param ImpactResult - The result of the hit, containing information about the impact.
	 * @param ImpactVelocity - The velocity of the projectile at the time of impact.
	 */
	UFUNCTION(BlueprintNativeEvent)
	void OnProjectileHit(const FHitResult& ImpactResult, const FVector& ImpactVelocity);

	/**
	 * Called when the projectile's lifespan ends.
	 */
	UFUNCTION(BlueprintNativeEvent)
	void OnProjectileLifeSpanEnd();
	
private:
	/**
	 * Processes the projectile's lifespan, decrementing it based on the DeltaSeconds.
	 * @param DeltaSeconds - The time elapsed since the last frame.
	 */
	void ProcessProjectileLifeSpan(const float DeltaSeconds);

	/**
	 * Ends the projectile's lifespan, cleaning up and notifying listeners.
	 */
	UFUNCTION()
	void EndProjectileLifeSpan();

	/**
	 * Handles the projectile hit event.
	 * @param ImpactResult - The result of the hit, containing information about the impact.
	 * @param ImpactVelocity - The velocity of the projectile at the time of impact.
	 */
	UFUNCTION()
	void ProjectileHit(const FHitResult& ImpactResult, const FVector& ImpactVelocity);
};
