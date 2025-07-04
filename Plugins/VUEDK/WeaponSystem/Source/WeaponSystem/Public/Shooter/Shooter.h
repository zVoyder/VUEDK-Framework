// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ShootBarrel.h"
#include "Behaviours/ShooterBehaviourBase.h"
#include "Components/ActorComponent.h"
#include "Data/ShootData.h"
#include "Shooter.generated.h"

DEFINE_LOG_CATEGORY_STATIC(LogShooter, All, All);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class WEAPONSYSTEM_API UShooter : public UActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Instanced)
	UShooterBehaviourBase* ShooterBehaviour;

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	FShootData ShootData;
	
public:
	UShooter();

	/**
	 * Initializes the shooter with the specified shoot barrel.
	 * @param InShootBarrel - The shoot barrel to use for shooting.
	 */
	UFUNCTION(BlueprintCallable)
	void Init(UShootBarrel* InShootBarrel);

	/**
	 * Sets the owner pawn for the shooter.
	 * @param InOwner - The pawn to set as the owner.
	 */
	UFUNCTION(BlueprintCallable)
	void SetOwner(APawn* InOwner) const;

	/**
	 * Executes the shoot action.
	 * @return true if the shoot was successful, false otherwise.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure = false)
	bool Shoot() const;
	
	/**
	 * Gets the shoot data for this shooter.
	 * @return The FShootData struct containing shoot configuration.
	 */
	UFUNCTION(BlueprintPure)
	FShootData GetShootData() const;

	/**
	 * Called when the component ends play. Used for cleanup logic.
	 * @param EndPlayReason - The reason the component is ending play.
	 */
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	/**
	 * Called every frame to update the shooter component.
	 * @param DeltaTime - The time elapsed since the last tick.
	 * @param TickType - The type of tick this is.
	 * @param ThisTickFunction - The tick function struct.
	 */
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	/**
	 * Checks if the shooter is valid and ready for operations.
	 * @return true if valid, false otherwise.
	 */
	bool Check() const;
};
