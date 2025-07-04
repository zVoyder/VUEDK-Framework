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
	/**
	 * Creates and returns a save data object representing the current state of the object.
	 * @return Pointer to the created USaveData instance.
	 */
	virtual USaveData* CreateSaveData() = 0;
    
	/**
	 * Loads the state of the object from the provided save data.
	 * @param SavedData - The save data to load from.
	 * @return true if the data was loaded successfully, false otherwise.
	 */
	virtual bool LoadSaveData(USaveData* SavedData) = 0;
};
