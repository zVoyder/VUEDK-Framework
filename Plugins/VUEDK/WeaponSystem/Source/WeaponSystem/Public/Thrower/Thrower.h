// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/ArrowComponent.h"
#include "Throwables/ThrowableBase.h"
#include "Components/SceneComponent.h"
#include "Thrower.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class WEAPONSYSTEM_API UThrower : public USceneComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Thrower")
	FGameplayTag ThrowableTag;

private:
#if WITH_EDITORONLY_DATA
	UPROPERTY()
	UArrowComponent* ArrowComponent;
#endif
	
public:
	UThrower();

	/**
	 * Throws the throwable with the specified force and optional angular speed.
	 * @param Force - The force applied to the throwable when thrown.
	 * @param AngularSpeed - The angular speed applied to the throwable when thrown (default is 0.0f).
	 */
	UFUNCTION(BlueprintCallable)
	void Throw(const float Force, const float AngularSpeed = 0.0f) const;

	/**
	 * Throws the throwable at a specific direction with the specified force and angular speed.
	 * @param Force - The force applied to the throwable when thrown.
	 * @param AngularSpeed - The angular speed applied to the throwable when thrown (default is 0.0f).
	 * @param Direction - The direction in which the throwable is thrown, normalized to a unit vector.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure = false)
	void ThrowAtDirection(const float Force, const float AngularSpeed, const FVector& Direction) const;
	
protected:
	/**
	 * Gets the throwable instance associated with this thrower.
	 * @return Pointer to the throwable instance, or nullptr if not set.
	 */
	AThrowableBase* GetThrowable() const;
};
