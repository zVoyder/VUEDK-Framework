// Copyright VUEDK, Inc. All Rights Reserved.

#include "GOAPFactories.h"
#include "Data/GOAPAgentBehaviourData.h"
#include "Data/GOAPBlackboardData.h"
#include "Kismet2/KismetEditorUtilities.h"

UGOAPBehaviourFactory::UGOAPBehaviourFactory()
{
	SupportedClass = UGOAPAgentBehaviourData::StaticClass();
	bCreateNew = true;
}

UObject* UGOAPBehaviourFactory::FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
{
	return NewObject<UGOAPAgentBehaviourData>(InParent, InClass, InName, Flags);
}

FName UGOAPBehaviourFactory::GetNewAssetThumbnailOverride() const
{
	return FName("BehaviourIcon");
}

UGOAPBlackboardFactory::UGOAPBlackboardFactory()
{
	SupportedClass = UGOAPBlackboardData::StaticClass();
	bCreateNew = true;
}

UObject* UGOAPBlackboardFactory::FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
{
	return NewObject<UGOAPBlackboardData>(InParent, InClass, InName, Flags);
}

FName UGOAPBlackboardFactory::GetNewAssetThumbnailOverride() const
{
	return FName("BlackboardIcon");
}

UGOAPSensorFactory::UGOAPSensorFactory()
{
	SupportedClass = UAgentSensor::StaticClass();
	bCreateNew = true;
}

UObject* UGOAPSensorFactory::FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
{
	return FKismetEditorUtilities::CreateBlueprint(
		SupportedClass,
		InParent,
		InName,
		BPTYPE_Normal,
		UBlueprint::StaticClass(),
		UBlueprintGeneratedClass::StaticClass(),
		FName("GOAPBPFactory")
	);
}

UGOAPBeliefFactory::UGOAPBeliefFactory()
{
	SupportedClass = UAgentBelief::StaticClass();
	bCreateNew = true;
}

UObject* UGOAPBeliefFactory::FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
{
	return FKismetEditorUtilities::CreateBlueprint(
		SupportedClass,
		InParent,
		InName,
		BPTYPE_Normal,
		UBlueprint::StaticClass(),
		UBlueprintGeneratedClass::StaticClass(),
		FName("GOAPBPFactory")
	);
}

UGOAPActionFactory::UGOAPActionFactory()
{
	SupportedClass = UAgentAction::StaticClass();
	bCreateNew = true;
}

UObject* UGOAPActionFactory::FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
{
	return FKismetEditorUtilities::CreateBlueprint(
		SupportedClass,
		InParent,
		InName,
		BPTYPE_Normal,
		UBlueprint::StaticClass(),
		UBlueprintGeneratedClass::StaticClass(),
		FName("GOAPBPFactory")
	);
}

UGOAPGoalFactory::UGOAPGoalFactory()
{
	SupportedClass = UAgentGoal::StaticClass();
	bCreateNew = true;
}

UObject* UGOAPGoalFactory::FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
{
	return FKismetEditorUtilities::CreateBlueprint(
		SupportedClass,
		InParent,
		InName,
		BPTYPE_Normal,
		UBlueprint::StaticClass(),
		UBlueprintGeneratedClass::StaticClass(),
		FName("GOAPBPFactory")
	);
}
