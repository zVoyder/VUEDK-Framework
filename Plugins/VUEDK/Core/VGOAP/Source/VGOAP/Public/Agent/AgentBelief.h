// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "GOAPBlackboard.h"
#include "Base/GOAPTagObjectBase.h"
#include "UObject/Object.h"
#include "AgentBelief.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(
	FOnBeliefStateChanged,
	UAgentBelief*, Belief,
	bool, NewState
);

UCLASS(Abstract, Blueprintable, BlueprintType, EditInlineNew)
class VGOAP_API UAgentBelief : public UGOAPTagObjectBase
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FOnBeliefStateChanged OnBeliefStateChanged;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Tooltip = "Value to match for the belief to be considered verified."))
	bool bValue = true;

private:
	bool bLastState;
	
public:
	virtual void Init(UAgentContext* InContext) override;
	
	bool Evaluate();
	
	bool GetLastState() const;
	
protected:
	UFUNCTION(BlueprintNativeEvent)
	bool OnEvaluate(APawn* Pawn, AAIController* Controller, UGOAPBlackboard* Blackboard);
	
private:
	void SetupInitialState();
	
	void CheckState(const bool NewState);
};
