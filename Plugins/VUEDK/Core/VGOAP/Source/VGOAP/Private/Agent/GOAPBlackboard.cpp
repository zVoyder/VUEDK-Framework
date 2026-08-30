// Copyright VUEDK, Inc. All Rights Reserved.

#include "Agent/GOAPBlackboard.h"
#include "Internationalization/Internationalization.h"
#include "Blueprint/BlueprintExceptionInfo.h"

#define LOCTEXT_NAMESPACE "GOAPBlackboard"

void UGOAPBlackboard::Init(UGOAPBlackboardData* InBlackboardData)
{
	BlackboardData = InBlackboardData;
	BlackboardProperties = InBlackboardData->BlackboardProperties;
}

void UGOAPBlackboard::Reset()
{
	BlackboardProperties = BlackboardData->BlackboardProperties;
}

void UGOAPBlackboard::SetValueInt32(const FName KeyName, const int32 InValue)
{
	BlackboardProperties.SetValueInt32(KeyName, InValue);
}

bool UGOAPBlackboard::GetValueInt32(const FName KeyName, int32& OutValue) const
{
	const TValueOrError<int32, EPropertyBagResult> Value = BlackboardProperties.GetValueInt32(KeyName);

	if (Value.HasError())
		return false;

	OutValue = Value.GetValue();
	return true;
}

void UGOAPBlackboard::SetValueInt64(const FName KeyName, const int64 InValue)
{
	BlackboardProperties.SetValueInt64(KeyName, InValue);
}

bool UGOAPBlackboard::GetValueInt64(const FName KeyName, int64& OutValue) const
{
	const TValueOrError<int64, EPropertyBagResult> Value = BlackboardProperties.GetValueInt64(KeyName);

	if (Value.HasError())
		return false;

	OutValue = Value.GetValue();
	return true;
}

void UGOAPBlackboard::SetValueFloat(const FName KeyName, const float InValue)
{
	BlackboardProperties.SetValueFloat(KeyName, InValue);
}

bool UGOAPBlackboard::GetValueFloat(const FName KeyName, float& OutValue) const
{
	const TValueOrError<float, EPropertyBagResult> Value = BlackboardProperties.GetValueFloat(KeyName);

	if (Value.HasError())
		return false;

	OutValue = Value.GetValue();
	return true;
}

void UGOAPBlackboard::SetValueString(const FName KeyName, const FString InValue)
{
	BlackboardProperties.SetValueString(KeyName, InValue);
}

bool UGOAPBlackboard::GetValueString(const FName KeyName, FString& OutValue) const
{
	const TValueOrError<FString, EPropertyBagResult> Value = BlackboardProperties.GetValueString(KeyName);

	if (Value.HasError())
		return false;

	OutValue = Value.GetValue();
	return true;
}

void UGOAPBlackboard::SetValueText(const FName KeyName, const FText InValue)
{
	BlackboardProperties.SetValueText(KeyName, InValue);
}

bool UGOAPBlackboard::GetValueText(const FName KeyName, FText& OutValue) const
{
	const TValueOrError<FText, EPropertyBagResult> Value = BlackboardProperties.GetValueText(KeyName);

	if (Value.HasError())
		return false;

	OutValue = Value.GetValue();
	return true;
}

void UGOAPBlackboard::SetValueName(const FName KeyName, const FName InValue)
{
	BlackboardProperties.SetValueName(KeyName, InValue);
}

bool UGOAPBlackboard::GetValueName(const FName KeyName, FName& OutValue) const
{
	const TValueOrError<FName, EPropertyBagResult> Value = BlackboardProperties.GetValueName(KeyName);

	if (Value.HasError())
		return false;

	OutValue = Value.GetValue();
	return true;
}

void UGOAPBlackboard::SetValueVector(const FName KeyName, const FVector InValue)
{
	BlackboardProperties.SetValueStruct(KeyName, InValue);
}

bool UGOAPBlackboard::GetValueVector(const FName KeyName, FVector& OutValue) const
{
	const TValueOrError<FStructView, EPropertyBagResult> Value = BlackboardProperties.GetValueStruct(KeyName);

	if (Value.HasError())
		return false;

	OutValue = *reinterpret_cast<const FVector*>(Value.GetValue().GetMemory());
	return true;
}

void UGOAPBlackboard::SetValueRotator(const FName KeyName, const FRotator InValue)
{
	BlackboardProperties.SetValueStruct(KeyName, InValue);
}

bool UGOAPBlackboard::GetValueRotator(const FName KeyName, FRotator& OutValue) const
{
	const TValueOrError<FStructView, EPropertyBagResult> Value = BlackboardProperties.GetValueStruct(KeyName);

	if (Value.HasError())
		return false;

	OutValue = *reinterpret_cast<const FRotator*>(Value.GetValue().GetMemory());
	return true;
}

void UGOAPBlackboard::SetValueTransform(const FName KeyName, const FTransform InValue)
{
	BlackboardProperties.SetValueStruct(KeyName, InValue);
}

bool UGOAPBlackboard::GetValueTransform(const FName KeyName, FTransform& OutValue) const
{
	const TValueOrError<FStructView, EPropertyBagResult> Value = BlackboardProperties.GetValueStruct(KeyName);

	if (Value.HasError())
		return false;

	OutValue = *reinterpret_cast<const FTransform*>(Value.GetValue().GetMemory());
	return true;
}

void UGOAPBlackboard::SetValueClass(const FName KeyName, const TSubclassOf<UObject> InValue)
{
	BlackboardProperties.SetValueClass(KeyName, InValue);
}

bool UGOAPBlackboard::GetValueClass(const FName KeyName, TSubclassOf<UObject>& OutValue) const
{
	const TValueOrError<UClass*, EPropertyBagResult> Value = BlackboardProperties.GetValueClass(KeyName);

	if (Value.HasError())
		return false;

	OutValue = Value.GetValue();
	return true;
}

void UGOAPBlackboard::SetValueBool(const FName KeyName, const bool InValue)
{
	BlackboardProperties.SetValueBool(KeyName, InValue);
}

bool UGOAPBlackboard::GetValueBool(const FName KeyName, bool& OutValue) const
{
	const TValueOrError<bool, EPropertyBagResult> Value = BlackboardProperties.GetValueBool(KeyName);

	if (Value.HasError())
		return false;

	OutValue = Value.GetValue();
	return true;
}

void UGOAPBlackboard::SetValueByte(const FName KeyName, const uint8 InValue)
{
	BlackboardProperties.SetValueByte(KeyName, InValue);
}

bool UGOAPBlackboard::GetValueByte(const FName KeyName, uint8& OutValue) const
{
	const TValueOrError<uint8, EPropertyBagResult> Value = BlackboardProperties.GetValueByte(KeyName);

	if (Value.HasError())
		return false;

	OutValue = Value.GetValue();
	return true;
}

void UGOAPBlackboard::SetValueEnum(const FName KeyName, const uint8 InValue)
{
	BlackboardProperties.SetValueByte(KeyName, InValue);
}

bool UGOAPBlackboard::GetValueEnum(const FName KeyName, uint8& OutValue) const
{
	const TValueOrError<uint8, EPropertyBagResult> Value = BlackboardProperties.GetValueByte(KeyName);

	if (Value.HasError())
		return false;

	OutValue = Value.GetValue();
	return true;
}

void UGOAPBlackboard::SetValueStruct(FName KeyName, int32 InValue)
{
	checkNoEntry()
}

EBlackboardResult UGOAPBlackboard::GetValueStruct(const FName KeyName, int32& OutValue) const
{
	checkNoEntry();
	return EBlackboardResult::NotValid;
}

void UGOAPBlackboard::SetValueObject(const FName KeyName, UObject* InValue)
{
	BlackboardProperties.SetValueObject(KeyName, InValue);
}

bool UGOAPBlackboard::GetValueObject(const FName KeyName, UObject*& OutValue) const
{
	const TValueOrError<UObject*, EPropertyBagResult> Value = BlackboardProperties.GetValueObject(KeyName);

	if (Value.HasError())
		return false;

	OutValue = Value.GetValue();
	return true;
}

EBlackboardResult UGOAPBlackboard::GetValueObjectAs(const FName KeyName, UClass* ExpectedClass, UObject*& OutValue) const
{
	const TValueOrError<UObject*, EPropertyBagResult> RetrievedValue = BlackboardProperties.GetValueObject(KeyName);

	if (RetrievedValue.HasError())
		return EBlackboardResult::NotValid;

	UObject* RetrievedObject = RetrievedValue.GetValue();
	if (!RetrievedObject || !RetrievedObject->IsA(ExpectedClass))
	{
		OutValue = nullptr;
		return EBlackboardResult::NotValid;
	}

	OutValue = RetrievedObject;
	return EBlackboardResult::Valid;
}

DEFINE_FUNCTION(UGOAPBlackboard::execSetValueStruct)
{
	P_GET_PROPERTY(FNameProperty, KeyName);

	Stack.MostRecentPropertyAddress = nullptr;
	Stack.MostRecentPropertyContainer = nullptr;
	Stack.StepCompiledIn<FStructProperty>(nullptr);

	const FStructProperty* ValueProp = CastField<FStructProperty>(Stack.MostRecentProperty);
	const void* ValuePtr = Stack.MostRecentPropertyAddress;

	P_FINISH;

	if (!ValueProp || !ValuePtr)
	{
		const FBlueprintExceptionInfo ExceptionInfo(
			EBlueprintExceptionType::AccessViolation,
			LOCTEXT("GOAPBlackboard_SetValueStruct_InvalidProperty", "GOAPBlackboard::SetValueStruct - Invalid property type.")
		);

		FBlueprintCoreDelegates::ThrowScriptException(P_THIS, Stack, ExceptionInfo);
		return;
	}

	UGOAPBlackboard* Blackboard = CastChecked<UGOAPBlackboard>(P_THIS);
	TValueOrError<FStructView, EPropertyBagResult> ValueInBlackboard = Blackboard->BlackboardProperties.GetValueStruct(KeyName);

	if (!ValueInBlackboard.HasValue() || ValueInBlackboard.HasError())
	{
		const FBlueprintExceptionInfo ExceptionInfo(
			EBlueprintExceptionType::AbortExecution,
			LOCTEXT("GOAPBlackboard_SetValueStruct_KeyNotFound", "GOAPBlackboard::SetValueStruct - Key not found in blackboard.")
		);

		FBlueprintCoreDelegates::ThrowScriptException(P_THIS, Stack, ExceptionInfo);
		return;
	}

	const FStructView& StructViewPtr = ValueInBlackboard.GetValue();
	if (!StructViewPtr.IsValid() || !StructViewPtr.GetScriptStruct()->IsChildOf(ValueProp->Struct))
	{
		const FBlueprintExceptionInfo ExceptionInfo(
			EBlueprintExceptionType::AbortExecution,
			LOCTEXT("GOAPBlackboard_SetValueStruct_TypeMismatch", "GOAPBlackboard::SetValueStruct - Type mismatch for the struct to set.")
		);

		FBlueprintCoreDelegates::ThrowScriptException(P_THIS, Stack, ExceptionInfo);
		return;
	}

	P_NATIVE_BEGIN;

		FInstancedStruct InstancedStruct;
		InstancedStruct.InitializeAs(ValueProp->Struct, static_cast<const uint8*>(ValuePtr));
		const FConstStructView ConstStructView = FConstStructView(InstancedStruct.GetScriptStruct(), InstancedStruct.GetMemory());
		Blackboard->BlackboardProperties.SetValueStruct(KeyName, ConstStructView);

	P_NATIVE_END;
}

DEFINE_FUNCTION(UGOAPBlackboard::execGetValueStruct)
{
	P_GET_PROPERTY(FNameProperty, KeyName);

	Stack.MostRecentPropertyAddress = nullptr;
	Stack.MostRecentPropertyContainer = nullptr;
	Stack.StepCompiledIn<FStructProperty>(nullptr);

	const FStructProperty* ValueProp = CastField<FStructProperty>(Stack.MostRecentProperty);
	void* ValuePtr = Stack.MostRecentPropertyAddress;

	P_FINISH;

	if (!ValueProp || !ValuePtr)
	{
		const FBlueprintExceptionInfo ExceptionInfo(
			EBlueprintExceptionType::AccessViolation,
			LOCTEXT("GOAPBlackboard_GetValueStruct_InvalidProperty", "GOAPBlackboard::GetValueStruct - Invalid property type.")
		);

		FBlueprintCoreDelegates::ThrowScriptException(P_THIS, Stack, ExceptionInfo);
		return;
	}

	EBlackboardResult Valid = EBlackboardResult::NotValid;
	
	P_NATIVE_BEGIN;

		const UGOAPBlackboard* Blackboard = CastChecked<UGOAPBlackboard>(P_THIS);
		TValueOrError<FStructView, EPropertyBagResult> ValueInBlackboard = Blackboard->BlackboardProperties.GetValueStruct(KeyName);

		if (ValueInBlackboard.HasValue() && !ValueInBlackboard.HasError())
		{
			const FStructView& StructViewPtr = ValueInBlackboard.GetValue();

			if (StructViewPtr.IsValid() && StructViewPtr.GetScriptStruct()->IsChildOf(ValueProp->Struct))
			{
				ValueProp->Struct->CopyScriptStruct(ValuePtr, StructViewPtr.GetMemory());
				Valid = EBlackboardResult::Valid;
			}
		}

	P_NATIVE_END;

	*static_cast<EBlackboardResult*>(RESULT_PARAM) = Valid;
}

#undef LOCTEXT_NAMESPACE
