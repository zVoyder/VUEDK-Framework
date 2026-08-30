// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"
#include "SSSerializationUtility.generated.h"

class USaveDataBase;
class FMemoryWriter;
class FMemoryReader;

UCLASS()
class SAVESYSTEM_API USSSerializationUtility : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Save System|Serialization")
	static bool TrySerializeSaveDataObjectInSaveGame(USaveDataBase* SaveDataToSerialize, bool bIsSharedSave = false);

	UFUNCTION(BlueprintCallable, Category = "Save System|Serialization")
	static bool TryDeserializeSaveDataObjectFromSaveGame(USaveDataBase* SaveDataToDeserialize, bool bIsSharedSave = false);

	UFUNCTION(BlueprintCallable, Category = "Save System|Serialization")
	static bool TrySerializeObjectInSaveGame(UObject* ObjectToSerialize, const FName ObjectId, bool bIsSharedSave = false);

	UFUNCTION(BlueprintCallable, Category = "Save System|Serialization")
	static bool TryDeserializeObjectFromSaveGame(UObject* ObjectToDeserialize, const FName ObjectId, bool bIsSharedSave = false);

private:
	static void SerializeAsSaveGame(UObject* ObjectToSerialize, FObjectAndNameAsStringProxyArchive& Ar);
};
