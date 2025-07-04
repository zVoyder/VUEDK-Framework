// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ShooterHandlerBase.h"
#include "UObject/Object.h"
#include "CooldownHandler.generated.h"

UCLASS(BlueprintType)
class WEAPONSYSTEM_API UCooldownHandler : public UShooterHandlerBase
{
	GENERATED_BODY()

private:
	bool bIsInCooldown;
	float CooldownRemaining;

public:
	virtual void Tick(float DeltaTime) override;
	
	void ResetCooldown();

	void StartCooldown();
	
	bool IsInCooldown() const;
	
private:
	void ProcessCooldown(float DeltaTime);

	void EndShootCooldown();
};
