// Copyright VUEDK, Inc. All Rights Reserved.

#include "Utility/BetterUIUtility.h"

FString UBetterUIUtility::GetProjectVersion()
{
	FString AppVersion;
	
	GConfig->GetString(
		TEXT("/Script/EngineSettings.GeneralProjectSettings"),
		TEXT("ProjectVersion"),
		AppVersion,
		GGameIni
	);

	return AppVersion;
}

FString UBetterUIUtility::GetProjectName()
{
	return FApp::GetProjectName();
}
