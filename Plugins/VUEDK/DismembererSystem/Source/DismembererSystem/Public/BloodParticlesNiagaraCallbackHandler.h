// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Dismemberer.h"
#include "NiagaraPerfBaseline.h"
#include "BloodParticlesNiagaraCallbackHandler.generated.h"

UCLASS()
class DISMEMBERERSYSTEM_API UBloodParticlesNiagaraCallbackHandler : public UObject, public INiagaraParticleCallbackHandler
{
	GENERATED_BODY()

private:
	UPROPERTY()
	UDismemberer* Dismemberer;
	bool bIsDecalInCooldown = false;
	
public:
	UBloodParticlesNiagaraCallbackHandler();
	
	virtual void ReceiveParticleData_Implementation(const TArray<FBasicParticleData>& Data, UNiagaraSystem* NiagaraSystem, const FVector& SimulationPositionOffset) override;

	/**
	 * Initializes the callback handler with the given Dismemberer.
	 * @param InDismemberer The Dismemberer to associate with this handler.
	 */
	void Init(UDismemberer* InDismemberer);

	/**
	 * Gets the Niagara callback name for this handler.
	 * @return The callback name as FName.
	 */
	FName GetNiagaraCallbackName() const;
	
private:
	/**
	 * Resets the cooldown for spawning decals.
	 */
	void ResetDecalSpawnCooldown();

	/**
	 * Checks the validity or state of the handler.
	 * @return True if valid, false otherwise.
	 */
	bool Check() const;
};
