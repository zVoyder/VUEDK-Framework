// Copyright VUEDK, Inc. All Rights Reserved.

#include "SimplePerceptionSystem.h"

#define LOCTEXT_NAMESPACE "FSimplePerceptionSystemModule"

DEFINE_LOG_CATEGORY(LogSimplePerceptionSystem);

TAutoConsoleVariable<bool> FSimplePerceptionSystemModule::CVarDebugPerceptionSystem(
	TEXT("SimplePerceptionSystem.Debug"),
	false,
	TEXT("Draw debug information for the perception system.\n"),
	ECVF_Default
);

void FSimplePerceptionSystemModule::StartupModule()
{
    
}

void FSimplePerceptionSystemModule::ShutdownModule()
{
    
}

#undef LOCTEXT_NAMESPACE
    
IMPLEMENT_MODULE(FSimplePerceptionSystemModule, SimplePerceptionSystem)
