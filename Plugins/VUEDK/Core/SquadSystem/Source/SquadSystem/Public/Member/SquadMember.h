// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "LeaderController.h"
#include "MemberEvaluator.h"
#include "MembershipHandler.h"
#include "Components/ActorComponent.h"
#include "Data/SquadInfo.h"
#include "Squad/Squad.h"
#include "SquadMember.generated.h"

class USquad;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
	FOnJoinedSquad,
	FName, SquadID
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(
	FOnLeftSquad
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
	FOnLeadershipChanged,
	bool, bIsLeader
);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SQUADSYSTEM_API USquadMember : public UActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FOnJoinedSquad OnJoinedSquad;
	UPROPERTY(BlueprintAssignable)
	FOnLeftSquad OnLeftSquad;
	UPROPERTY(BlueprintAssignable, BlueprintAuthorityOnly)
	FOnLeadershipChanged OnLeadershipChanged;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Instanced)
	UMembershipHandler* MembershipHandler;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Instanced)
	UMemberEvaluator* MemberEvaluator;
	UPROPERTY(EditDefaultsOnly)
	bool bCanBeLeader;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Instanced, meta = (EditCondition = "bCanBeLeader", EditConditionHides))
	ULeaderController* LeaderController;

private:
	UPROPERTY()
	USquad* OwnerSquad;
	UPROPERTY()
	TArray<UMemberObjectBase*> MemberObjects;
	
	UPROPERTY(ReplicatedUsing=OnRep_SquadID)
	FName OwnerSquadID;
	UPROPERTY(Replicated)
	bool bIsLeader;

public:
	USquadMember();

	virtual void TickComponent(const float DeltaTime, const ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void ChangeLeadership(const bool bNewIsLeader);

	void AssignSquad(USquad* NewSquad);
	
	void ClearSquad();

	UFUNCTION(BlueprintPure)
	FName GetOwnerSquadID() const;
	
	UFUNCTION(BlueprintPure)
	bool HasSquad() const;

	UFUNCTION(BlueprintPure)
	bool IsLeader() const;
	
	UFUNCTION(BlueprintPure, BlueprintAuthorityOnly)
	USquad* GetOwnerSquad() const;
	
	UFUNCTION(BlueprintPure, BlueprintAuthorityOnly)
	USquadMember* GetOwnerSquadLeader() const;
	
	UFUNCTION(BlueprintPure, BlueprintAuthorityOnly)
	bool CanBeLeader() const;

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	bool JoinSquad(USquadMember* Target, const TEnumAsByte<ESquadResolution> Resolution);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	bool LeaveSquad(const TEnumAsByte<ESquadResolution> Resolution);

protected:
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

private:
	void Init();

	void InitMemberObjects();
	
	void JoinMemberObjects(USquad* NewSquad) const;

	void LeaveMemberObjects(USquad* LeftSquad) const;
	
	void TickMemberObjects(float DeltaTime) const;

	void BeginPlayMemberObjects() const;

	void EndPlayMemberObjects(EEndPlayReason::Type EndPlayReason) const;
	
	bool Check() const;

	UFUNCTION()
	void OnRep_SquadID() const;

#if !UE_BUILD_SHIPPING
	void DrawDebugMemberState() const;
#endif
};
