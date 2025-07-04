// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Data/SaveData.h"
#include "UObject/Object.h"
#include "Saveable.generated.h"

UINTERFACE(BlueprintType)
class SAVESYSTEM_API USaveable : public UInterface
{
	GENERATED_BODY()
};

class ISaveable
{
	GENERATED_BODY()

public:
	virtual USaveData* CreateSaveData() = 0;
    
	virtual bool LoadSaveData(USaveData* SavedData) = 0;
};
