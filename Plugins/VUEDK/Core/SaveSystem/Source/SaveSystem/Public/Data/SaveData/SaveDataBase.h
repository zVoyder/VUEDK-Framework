// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Utility/SSSerializationUtility.h"
#include "SaveDataBase.generated.h"

UCLASS(Abstract, Blueprintable, BlueprintType)
class SAVESYSTEM_API USaveDataBase : public UObject
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, meta = (ExposeOnSpawn = true))
	FName SaveDataID = "DefaultID";

public:
	UFUNCTION(BlueprintCallable)
	virtual bool SaveObjectData(UObject* ObjectToSave);

	UFUNCTION(BlueprintCallable)
	virtual bool LoadObjectData(UObject* ObjectToLoad);
	
	UFUNCTION(BlueprintCallable)
	void SetSaveDataID(const FName NewID);

	UFUNCTION(BlueprintCallable)
	FName GetSaveDataId() const;
	
protected:
	UFUNCTION(BlueprintNativeEvent, DisplayName = "SaveObjectData")
	bool ReceiveSaveObjectData(UObject* ObjectToSave);

	UFUNCTION(BlueprintNativeEvent, DisplayName = "LoadObjectData")
	bool ReceiveLoadObjectData(UObject* ObjectToLoad);
	
	virtual void Serialize(FArchive& Ar) override;
	
	void SerializeSaveGameMembers(FArchive& Ar);
	
	void HandleObjectProperty(FArchive& Ar, const FObjectProperty* ObjProp);
	
	void HandleArrayProperty(FArchive& Ar, const FArrayProperty* ArrayProp);
	
	void HandleMapProperty(FArchive& Ar, const FMapProperty* MapProp);
	
	void HandleSetProperty(FArchive& Ar, const FSetProperty* SetProp);
};
