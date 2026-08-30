// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "SquadEvaluator.h"
#include "SquadSubsystem.h"
#include "Squad/SquadHandler.h"
#include "SquadFactory.generated.h"

UCLASS()
class SQUADSYSTEM_API USquadFactory : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	static USquad* CreateSquad(USquadSubsystem* SquadSubsystem, FName SquadID, const bool bLockedState, TArray<TSoftClassPtr<USquadHandler>> HandlerClasses);

	static USquadEvaluator* CreateSquadEvaluator(USquadSubsystem* SquadSubsystem, const TSubclassOf<USquadEvaluator> EvaluatorClass);

	UFUNCTION(BlueprintCallable, Category="Squad System|Factory")
	static USquadHandler* CreateSquadHandler(USquad* OwnerSquad, const TSubclassOf<USquadHandler> HandlerClass);
};
