// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Montages/Data/WeaponAnimMetaData.h"
#include "WeaponAnimFactory.generated.h"

UCLASS()
class WEAPONSYSTEM_API UWeaponAnimFactory : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	static UWeaponAnimMetaData* CreateWeaponAnimMetaData(AWeaponBase* InWeapon);
};
