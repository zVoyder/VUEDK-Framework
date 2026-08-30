// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Data/SaveData/SaveDataBase.h"
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
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Saveable")
	USaveDataBase* CreateSaveDataInstance();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Saveable")
	void PrepareForSerialization(USaveDataBase* SaveData);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Saveable")
	void PrepareForDeserialization(USaveDataBase* SaveData);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Saveable")
	bool Save(USaveDataBase* SaveData);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Saveable")
	bool Load(USaveDataBase* SaveData);
};
