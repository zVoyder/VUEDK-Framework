// Copyright zVoyder, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "PooledActorBase.h"
#include "Thrower/Data/EThrowableSpinAxis.h"
#include "ThrowableBase.generated.h"

UCLASS(Abstract, Blueprintable, BlueprintType)
class WEAPONSYSTEM_API AThrowableBase : public APooledActorBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Throwable")
	float LifeTime = 5.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Throwable")
	TEnumAsByte<EThrowableSpinAxis> ThrowableSpinAxis;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UStaticMeshComponent* MeshComponent;
	UPROPERTY()
	AActor* ThrowableInstigator;
	FTimerHandle LifeSpanHandle;

public:
	AThrowableBase();

	/**
	 * Initializes the throwable with the specified instigator.
	 * @param InInstigator The actor that instigated the throwable.
	 */
	void Init(AActor* InInstigator);
	
	/**
	 * Gets the mesh component of the throwable.
	 * @return Pointer to the UStaticMeshComponent.
	 */
	UStaticMeshComponent* GetMeshComponent() const;
	
	/**
	 * Gets the spin axis vector for the throwable.
	 * @return The spin axis as a FVector.
	 */
	FVector GetSpinAxis() const;

	/**
	 * Gets a custom axis spin for the throwable (can be overridden in Blueprints).
	 * @return The custom axis spin as a FVector.
	 */
	UFUNCTION(BlueprintNativeEvent)
	FVector GetCustomAxisSpin() const;
	
protected:
	/**
	 * Called when the pooled actor begins play (object pool integration).
	 */
	virtual void OnPooledActorBeginPlay_Implementation() override;

	/**
	 * Clears the pooled actor (object pool integration).
	 */
	virtual void ClearPooledActor_Implementation() override;

	/**
	 * Invalidates the life span timer for the throwable.
	 */
	void InvalidateLifeSpanTimer();
	
	/**
	 * Called when the throwable's life span ends (can be overridden in Blueprints).
	 */
	UFUNCTION(BlueprintNativeEvent)
	void OnThrowableLifeSpanEnd();

private:
	/**
	 * Called when the throwable's life span ends (internal logic).
	 */
	UFUNCTION()
	void EndThrowableLifeSpan();
	
	/**
	 * Sets the life span for the throwable.
	 * @param LifeSpan The life span value to set.
	 */
	void SetLifeSpan(const float LifeSpan);
};
