// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ShooterHandlerBase.h"
#include "UObject/Object.h"
#include "RecoilHandler.generated.h"

UCLASS(BlueprintType)
class WEAPONSYSTEM_API URecoilHandler : public UShooterHandlerBase
{
	GENERATED_BODY()

private:
	float RecoilRemaining;
	FRotator ImpulseRecoil;

public:
	virtual void Tick(float DeltaTime) override;

	void ApplyRecoilImpulse();

	void ProcessRecoilImpulseRotation(float DeltaTime);

protected:
	virtual bool Check() const override;
};
