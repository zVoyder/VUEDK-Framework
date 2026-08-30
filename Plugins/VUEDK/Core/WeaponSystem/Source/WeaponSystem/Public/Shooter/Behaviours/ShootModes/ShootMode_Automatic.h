// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Base/ShootModeBase.h"
#include "ShootMode_Automatic.generated.h"

UCLASS()
class WEAPONSYSTEM_API UShootMode_Automatic : public UShootModeBase
{
	GENERATED_BODY()

protected:
	virtual bool OnRequestShoot_Implementation(EShootType ShootType) override;
};
