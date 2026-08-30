// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Base/ShooterHandlerBase.h"
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
	virtual void TickHandler(float DeltaTime) override;

	/**
	 * Applies a recoil impulse to the handler, starting the recoil effect.
	 */
	void ApplyRecoilImpulse();

	/**
	 * Processes the recoil impulse rotation each frame.
	 * @param DeltaTime The time elapsed since the last tick.
	 */
	void ProcessRecoilImpulseRotation(float DeltaTime);
};
