// Copyright VUEDK, Inc. All Rights Reserved.

#include "RPGSystem.h"

#define LOCTEXT_NAMESPACE "FRPGSystemModule"

DEFINE_LOG_CATEGORY(LogRPGSystem);
DEFINE_LOG_CATEGORY(LogStatsSystem);

void FRPGSystemModule::StartupModule()
{
}

void FRPGSystemModule::ShutdownModule()
{
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FRPGSystemModule, RPGSystem)