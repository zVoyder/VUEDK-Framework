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
	
	void Init(AActor* InInstigator, const TSubclassOf<UDamageType>& InDamageTypeClass, float InDamage, float InRange, float InSpeed, const FVector& InDirection);

	virtual void OnPooledActorEndPlay_Implementation() override;
	
	UFUNCTION(BlueprintNativeEvent)
	void InitVelocityAndLifeSpan(float InRange, float InSpeed, const FVector& InDirection);

	UFUNCTION(BlueprintNativeEvent)
	float CalculateLifeSpan(const float InRange, const float InSpeed) const;
	
	UFUNCTION(BlueprintCallable)
	void SetDamage(float InDamage);

	UFUNCTION(BlueprintCallable)
	void SetRange(float InRange);

	UFUNCTION(BlueprintCallable)
	void SetVelocity(const FVector NewVelocity) const;

	UFUNCTION(BlueprintCallable)
	void SetProjectileInstigator(AActor* InInstigator);

	UFUNCTION(BlueprintPure)
	float GetDamage() const;

	UFUNCTION(BlueprintPure)
	float GetRange() const;

	UFUNCTION(BlueprintPure)
	FVector GetProjectileVelocity() const;

	UFUNCTION(BlueprintPure)
	AActor* GetProjectileInstigator() const;

	UFUNCTION(BlueprintCallable)
	void DisposeProjectile();

	virtual void ClearPooledActor_Implementation() override;

protected:
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
	void StartProjectileLifeSpan(const float InLifeSpan);

	UFUNCTION(BlueprintNativeEvent)
	void OnInit();
	
	UFUNCTION(BlueprintNativeEvent)
	void OnProjectileHit(const FHitResult& ImpactResult, const FVector& ImpactVelocity);

	UFUNCTION(BlueprintNativeEvent)
	void OnProjectileLifeSpanEnd();
	
private:
	void ProcessProjectileLifeSpan(const float DeltaSeconds);
	
	UFUNCTION()
	void EndProjectileLifeSpan();
	
	UFUNCTION()
	void ProjectileHit(const FHitResult& ImpactResult, const FVector& ImpactVelocity);
};
