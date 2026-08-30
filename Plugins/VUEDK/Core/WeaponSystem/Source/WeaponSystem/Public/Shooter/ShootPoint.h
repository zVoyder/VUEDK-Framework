// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#if WITH_EDITOR
#include "Components/ArrowComponent.h"
#endif

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "ShootPoint.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), NotBlueprintable)
class WEAPONSYSTEM_API UShootPoint : public USceneComponent
{
	GENERATED_BODY()

private:
	float SpreadRad;
#if WITH_EDITORONLY_DATA
	UPROPERTY()
	UArrowComponent* ArrowComponent;
#endif

public:
	UShootPoint();

	/**
	 * Sets the spread in degrees for this shoot point.
	 * @param NewSpreadDegree The new spread value in degrees.
	 */
	void SetSpreadDegree(const float NewSpreadDegree);
	
	/**
	 * Generates a random spread value up to the specified maximum degree.
	 * @param MaxSpreadDegree The maximum spread value in degrees.
	 */
	void GenerateSpreadDegree(const float MaxSpreadDegree);

	/**
	 * Gets the spread in radians for this shoot point.
	 * @return The spread value in radians.
	 */
	UFUNCTION(BlueprintPure)
	float GetSpreadRad() const;
	
	/**
	 * Gets the world location of the shoot point.
	 * @return The world location as a FVector.
	 */
	UFUNCTION(BlueprintPure)
	FVector GetShootPointLocation() const;

	/**
	 * Gets the relative location of the shoot point.
	 * @return The relative location as a FVector.
	 */
	UFUNCTION(BlueprintPure)
	FVector GetShootPointRelativeLocation() const;

	/**
	 * Gets the world direction of the shoot point.
	 * @return The world direction as a FVector.
	 */
	UFUNCTION(BlueprintPure)
	FVector GetShootPointDirection() const;

	/**
	 * Gets the relative direction of the shoot point.
	 * @return The relative direction as a FVector.
	 */
	UFUNCTION(BlueprintPure)
	FVector GetShootPointRelativeDirection() const;

	/**
	 * Gets the world direction of the shoot point with spread applied.
	 * @return The spreaded world direction as a FVector.
	 */
	UFUNCTION(BlueprintPure)
	FVector GetShootPointSpreadedDirection() const;

	/**
	 * Gets the relative direction of the shoot point with spread applied.
	 * @return The spreaded relative direction as a FVector.
	 */
	UFUNCTION(BlueprintPure)
	FVector GetShootPointSpreadedRelativeDirection() const;

protected:
	/**
	 * Called when the component is registered with the engine.
	 */
	virtual void OnRegister() override;
};
