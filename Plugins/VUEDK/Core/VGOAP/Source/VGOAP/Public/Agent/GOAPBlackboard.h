// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Data/GOAPBlackboardData.h"
#include "StructUtils/PropertyBag.h"
#include "UObject/Object.h"
#include "GOAPBlackboard.generated.h"

UENUM()
enum class EBlackboardResult : uint8
{
	Valid,
	NotValid,
};

UCLASS(NotBlueprintable, BlueprintType)
class VGOAP_API UGOAPBlackboard : public UObject
{
	GENERATED_BODY()

private:
	UPROPERTY()
	TObjectPtr<UGOAPBlackboardData> BlackboardData;
	FInstancedPropertyBag BlackboardProperties;

public:
	void Init(UGOAPBlackboardData* InBlackboardData);

	UFUNCTION(BlueprintCallable)
	void Reset();
	
	UFUNCTION(BlueprintCallable)
	void SetValueInt32(const FName KeyName, const int32 InValue);

	UFUNCTION(BlueprintPure)
	bool GetValueInt32(const FName KeyName, int32& OutValue) const;

	UFUNCTION(BlueprintCallable)
	void SetValueInt64(const FName KeyName, const int64 InValue);

	UFUNCTION(BlueprintPure)
	bool GetValueInt64(const FName KeyName, int64& OutValue) const;

	UFUNCTION(BlueprintCallable)
	void SetValueFloat(const FName KeyName, const float InValue);

	UFUNCTION(BlueprintPure)
	bool GetValueFloat(const FName KeyName, float& OutValue) const;

	UFUNCTION(BlueprintCallable)
	void SetValueString(const FName KeyName, const FString InValue);

	UFUNCTION(BlueprintPure)
	bool GetValueString(const FName KeyName, FString& OutValue) const;

	UFUNCTION(BlueprintCallable)
	void SetValueText(const FName KeyName, const FText InValue);

	UFUNCTION(BlueprintPure)
	bool GetValueText(const FName KeyName, FText& OutValue) const;

	UFUNCTION(BlueprintCallable)
	void SetValueName(const FName KeyName, const FName InValue);

	UFUNCTION(BlueprintPure)
	bool GetValueName(const FName KeyName, FName& OutValue) const;

	UFUNCTION(BlueprintCallable)
	void SetValueVector(const FName KeyName, const FVector InValue);

	UFUNCTION(BlueprintPure)
	bool GetValueVector(const FName KeyName, FVector& OutValue) const;

	UFUNCTION(BlueprintCallable)
	void SetValueRotator(const FName KeyName, const FRotator InValue);

	UFUNCTION(BlueprintPure)
	bool GetValueRotator(const FName KeyName, FRotator& OutValue) const;

	UFUNCTION(BlueprintCallable)
	void SetValueTransform(const FName KeyName, const FTransform InValue);

	UFUNCTION(BlueprintPure)
	bool GetValueTransform(const FName KeyName, FTransform& OutValue) const;

	UFUNCTION(BlueprintCallable)
	void SetValueClass(const FName KeyName, const TSubclassOf<UObject> InValue);

	UFUNCTION(BlueprintPure)
	bool GetValueClass(const FName KeyName, TSubclassOf<UObject>& OutValue) const;
	
	UFUNCTION(BlueprintCallable)
	void SetValueBool(const FName KeyName, const bool InValue);
	
	UFUNCTION(BlueprintPure)
	bool GetValueBool(const FName KeyName, bool& OutValue) const;

	UFUNCTION(BlueprintCallable)
	void SetValueByte(const FName KeyName, const uint8 InValue);

	UFUNCTION(BlueprintPure)
	bool GetValueByte(const FName KeyName, uint8& OutValue) const;

	UFUNCTION(BlueprintCallable)
	void SetValueEnum(const FName KeyName, UPARAM(Ref) const uint8 InValue);

	UFUNCTION(BlueprintPure)
	bool GetValueEnum(const FName KeyName, uint8& OutValue) const;

	UFUNCTION(BlueprintCallable, CustomThunk, meta = (CustomStructureParam = "InValue"))
	void SetValueStruct(FName KeyName, int32 InValue);
	
	UFUNCTION(BlueprintCallable, BlueprintPure = false, CustomThunk, meta = (CustomStructureParam = "OutValue", ExpandEnumAsExecs = "ReturnValue"))
	EBlackboardResult GetValueStruct(const FName KeyName, int32& OutValue) const;

	UFUNCTION(BlueprintCallable)
	void SetValueObject(const FName KeyName, UObject* InValue);

	UFUNCTION(BlueprintPure)
	bool GetValueObject(const FName KeyName, UObject*& OutValue) const;
	
	UFUNCTION(BlueprintCallable, BlueprintPure = false, meta = (DeterminesOutputType = "ExpectedClass", DynamicOutputParam = "OutValue", ExpandEnumAsExecs = "ReturnValue"))
	EBlackboardResult GetValueObjectAs(const FName KeyName, UClass* ExpectedClass, UObject*& OutValue) const;

private:
	DECLARE_FUNCTION(execSetValueStruct);
	DECLARE_FUNCTION(execGetValueStruct);
};
