// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Shooter/Data/ShootData.h"
#include "ShooterHandlerBase.generated.h"

class UShootBarrel;
class UShooterBehaviourBase;

UCLASS(Abstract, NotBlueprintable)
class WEAPONSYSTEM_API UShooterHandlerBase : public UObject, public FTickableGameObject
{
	GENERATED_BODY()

protected:
	UPROPERTY()
	UShooterBehaviourBase* Behaviour;
	FShootData* ShootData;

private:
	bool bIsInitialized = false;

public:
	/**
	 * Initializes the shooter handler with the specified behaviour and shoot data.
	 * @param InBehaviour - The shooter behaviour to associate with this handler.
	 * @param InShootData - The shoot data to use for initialization.
	 */
	void Init(UShooterBehaviourBase* InBehaviour, FShootData& InShootData);
	
	virtual void Tick(float DeltaTime) override;
	
	virtual bool IsTickable() const override;
	
	virtual TStatId GetStatId() const override;

protected:
	/**
	 * Called when the handler is initialized. Can be overridden in subclasses.
	 */
	virtual void OnInit();

	/**
	 * Binds events to the handler. Can be overridden in subclasses.
	 */
	virtual void BindEvents();
	
	/**
	 * Called when the behaviour is enabled.
	 */
	UFUNCTION()
	virtual void OnBehaviourEnabled();
	
	/**
	 * Called when the behaviour is disabled.
	 */
	UFUNCTION()
	virtual void OnBehaviourDisabled();
	
	/**
	 * Called when the behaviour shoot is successful.
	 * @param ShootBarrel - The shoot barrel used.
	 * @param ShotIndex - The index of the shot.
	 */
	UFUNCTION()
	virtual void OnBehaviourShootSuccess(UShootBarrel* ShootBarrel, int32 ShotIndex);

	/**
	 * Called when the behaviour shoot fails.
	 */
	UFUNCTION()
	virtual void OnBehaviourShootFail();

	/**
	 * Checks if the handler is valid and ready for operations.
	 * @return true if valid, false otherwise.
	 */
	virtual bool Check() const;
};
