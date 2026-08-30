// Copyright VUEDK, Inc. All Rights Reserved.

#include "Factories/SquadFactory.h"
#include "Squad/Squad.h"

USquad* USquadFactory::CreateSquad(USquadSubsystem* SquadSubsystem, const FName SquadID, const bool bLockedState, TArray<TSoftClassPtr<USquadHandler>> HandlerClasses)
{
	USquad* NewSquad = NewObject<USquad>(SquadSubsystem);
	for (const TSoftClassPtr<USquadHandler>& HandlerClassPtr : HandlerClasses)
	{
		if (const TSubclassOf<USquadHandler> HandlerClass = HandlerClassPtr.LoadSynchronous())
			CreateSquadHandler(NewSquad, HandlerClass);
	}

	NewSquad->Init(SquadSubsystem, SquadID, bLockedState);
	return NewSquad;
}

USquadEvaluator* USquadFactory::CreateSquadEvaluator(USquadSubsystem* SquadSubsystem, const TSubclassOf<USquadEvaluator> EvaluatorClass)
{
	return NewObject<USquadEvaluator>(GetTransientPackage(), EvaluatorClass);
}

USquadHandler* USquadFactory::CreateSquadHandler(USquad* OwnerSquad, const TSubclassOf<USquadHandler> HandlerClass)
{
	USquadHandler* SquadHandler = NewObject<USquadHandler>(OwnerSquad, HandlerClass);
	SquadHandler->Init(OwnerSquad);
	OwnerSquad->AddHandler(SquadHandler);
	return SquadHandler;
}
