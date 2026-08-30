// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "SquadEvaluator.h"
#include "SquadSubsystem.h"
#include "Squad/SquadHandler.h"
#include "UObject/Object.h"
#include "SquadSystemSettings.generated.h"

UCLASS(Config = SquadSystemSettings, DefaultConfig, meta = (DisplayName = "Squad System Settings"))
class SQUADSYSTEM_API USquadSystemSettings : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(Config, EditAnywhere, Category = Settings)
	bool bDefaultSquadLockedState;
	UPROPERTY(Config, EditAnywhere, Category = Settings, meta = (ClampMin = "2", UIMin = "2"))
	int32 MaxMembersPerSquad = 5;
	UPROPERTY(Config, EditAnywhere, Category = Settings)
	float SquadTickInterval = 0.f;
	UPROPERTY(Config, EditAnywhere, Category = Settings)
	TEnumAsByte<EDefaultResolution> DefaultJoinResolution = EDefaultResolution::DefaultRebuild;
	UPROPERTY(Config, EditAnywhere, Category = Settings)
	TSoftClassPtr<USquadEvaluator> SquadEvaluatorClass = USquadEvaluator::StaticClass();
	UPROPERTY(Config, EditAnywhere, Category = Settings)
	TArray<TSoftClassPtr<USquadHandler>> SquadHandlerClasses;
};
