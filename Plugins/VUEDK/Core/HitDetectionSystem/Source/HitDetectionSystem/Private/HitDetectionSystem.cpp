// Copyright VUEDK, Inc. All Rights Reserved.

#include "HitDetectionSystem.h"

#define LOCTEXT_NAMESPACE "FHitBoxSystemModule"

DEFINE_LOG_CATEGORY(LogHitDetectionSystem);

void FHitDetectionSystemModule::StartupModule()
{
}

void FHitDetectionSystemModule::ShutdownModule()
{
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FHitDetectionSystemModule, HitDetectionSystem)
