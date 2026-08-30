// Copyright VUEDK, Inc. All Rights Reserved.

#include "EventsDispatcherSystem.h"

#define LOCTEXT_NAMESPACE "FEventsDispatcherSystemModule"

DEFINE_LOG_CATEGORY(LogEventsDispatcherSystem);

void FEventsDispatcherSystemModule::StartupModule()
{
}

void FEventsDispatcherSystemModule::ShutdownModule()
{
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FEventsDispatcherSystemModule, EventsDispatcherSystem)
