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
	
	void Init(UDismemberer* InDismemberer);

	FName GetNiagaraCallbackName() const;
	
private:
	void ResetDecalSpawnCooldown();

	bool Check() const;
};
