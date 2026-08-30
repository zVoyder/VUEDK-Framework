// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "GOAPObjectBase.generated.h"

class UGOAPBlackboard;
class AAIController;
class UGOAPAgent;
class UAgentContext;

/**
 * Base class for GOAP agent-related objects.
 */
UCLASS(Abstract, NotBlueprintable, NotBlueprintType)
class VGOAP_API UGOAPObjectBase : public UObject
{
	GENERATED_BODY()

private:
	UPROPERTY()
	UAgentContext* AgentContext;

public:
	virtual void Init(UAgentContext* InContext);

	UAgentContext* GetAgentContext() const;

	UFUNCTION(BlueprintPure)
	UGOAPAgent* GetAgent() const;

	UFUNCTION(BlueprintPure)
	AActor* GetAgentActor() const;

	UFUNCTION(BlueprintPure)
	APawn* GetAgentPawn() const;

	UFUNCTION(BlueprintPure)
	AAIController* GetAgentController() const;

	UFUNCTION(BlueprintPure)
	UGOAPBlackboard* GetAgentBlackboard() const;

	UFUNCTION(BlueprintPure)
	UGOAPBlackboard* GetAgentGlobalBlackboard() const;

#if WITH_EDITOR
	virtual bool ImplementsGetWorld() const override;
#endif

	virtual UWorld* GetWorld() const override;

protected:
	UFUNCTION(BlueprintNativeEvent)
	void OnInit();
};
