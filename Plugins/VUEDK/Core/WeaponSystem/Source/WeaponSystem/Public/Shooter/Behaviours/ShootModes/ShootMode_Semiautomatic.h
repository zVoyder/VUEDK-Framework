// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Base/ShootModeBase.h"
#include "ShootMode_Semiautomatic.generated.h"

UCLASS()
class WEAPONSYSTEM_API UShootMode_Semiautomatic : public UShootModeBase
{
	GENERATED_BODY()

protected:
	virtual bool OnRequestShoot_Implementation(EShootType ShootType) override;

	virtual void OnEndSequence_Implementation() override;
};
