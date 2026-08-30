// Copyright VUEDK, Inc. All Rights Reserved.

#include "InventorySaveBridge.h"

#define LOCTEXT_NAMESPACE "FInventorySaveBridgeModule"

DEFINE_LOG_CATEGORY(LogInventorySaveBridge);

void FInventorySaveBridgeModule::StartupModule()
{
}

void FInventorySaveBridgeModule::ShutdownModule()
{
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FInventorySaveBridgeModule, InventorySaveBridge)
