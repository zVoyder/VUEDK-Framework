// Copyright VUEDK, Inc. All Rights Reserved.

#include "AssetTypeActions_Sensor.h"
#include "VGOAPEditor.h"
#include "Agent/AgentSensor.h"

UClass* FAssetTypeActions_Sensor::GetSupportedClass() const
{
	return UAgentSensor::StaticClass();
}

FText FAssetTypeActions_Sensor::GetName() const
{
	return FText::FromString("Sensor");
}

const TArray<FText>& FAssetTypeActions_Sensor::GetSubMenus() const
{
	return FVGOAPEditorModule::GetGOAPSubMenus();
}
