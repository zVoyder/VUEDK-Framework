// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "SquadHandler.h"
#include "Member/LeaderController.h"
#include "UObject/Object.h"
#include "Squad.generated.h"

class USquadMember;

UENUM()
enum class ESquadHandlerResult : uint8
{
	Valid,
	NotValid,
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(
	FOnSquadMemberJoined,
	USquad*, Squad,
	USquadMember*, Member
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(
	FOnSquadMemberLeft,
	USquad*, Squad,
	USquadMember*, Member
);

UCLASS(NotBlueprintable, BlueprintType)
class SQUADSYSTEM_API USquad : public UObject
{
	GENERATED_BODY()

	friend class USquadSubsystem;

public:
	UPROPERTY(BlueprintAssignable)
	FOnSquadMemberJoined OnSquadMemberJoined;
	UPROPERTY(BlueprintAssignable)
	FOnSquadMemberLeft OnSquadMemberLeft;

private:
	UPROPERTY()
	USquadMember* Leader;
	UPROPERTY()
	TArray<USquadMember*> Members;
	UPROPERTY()
	TArray<USquadHandler*> Handlers;
	UPROPERTY()
	const USquadSubsystem* SquadSubsystem;
	bool bIsLocked;
	FName SquadID;

public:
	void Init(const USquadSubsystem* InSquadSubsystem, const FName InSquadID, const bool bLockedState = false);

	virtual void TickSquad(const float DeltaTime);
	
	void AddHandler(USquadHandler* NewHandler);

	void RemoveHandler(USquadHandler* HandlerToRemove);

	void ClearHandlers();

	FName GetSquadId() const;

	/**
	 * @brief Locks the squad, preventing it from accepting new members.
	 */
	UFUNCTION(BlueprintCallable)
	void Lock();

	/**
	 * @brief Unlocks the squad, allowing it to accept new members.
	 */
	UFUNCTION(BlueprintCallable)
	void Unlock();

	/**
	 * @brief Checks if the squad is locked, preventing it from accepting new members.
	 * @return True if the squad is locked, false otherwise.
	 */
	UFUNCTION(BlueprintPure)
	bool IsLocked() const;

	UFUNCTION(BlueprintCallable, BlueprintPure = false, meta = (DeterminesOutputType = "HandlerClass", DynamicOutputParam = "OutHandler", ExpandEnumAsExecs = "ReturnValue"))
	ESquadHandlerResult GetSquadHandler(const TSubclassOf<USquadHandler> HandlerClass, USquadHandler*& OutHandler) const;

	UFUNCTION(BlueprintPure)
	USquadMember* GetLeader() const;

	UFUNCTION(BlueprintPure)
	ULeaderController* GetLeaderController() const;

	UFUNCTION(BlueprintPure)
	bool HasLeader() const;

	UFUNCTION(BlueprintPure)
	const TArray<USquadMember*>& GetMembers() const;

	UFUNCTION(BlueprintPure)
	int32 GetMembersCount() const;

	UFUNCTION(BlueprintPure)
	bool IsFull() const;

	UFUNCTION(BlueprintPure)
	bool CanContainsMoreMembers(const int32 AdditionalMembers = 1) const;

	UFUNCTION(BlueprintPure)
	bool ContainsMember(USquadMember* Member) const;

	virtual UWorld* GetWorld() const override;

private:
	void AssignLeader(USquadMember* NewLeader);

	void RemoveLeader();

	bool TryAddMember(USquadMember* InMember, const bool bNotify = true);

	bool TryAddMembers(const TArray<USquadMember*>& InMembers, const bool bNotify = true);

	void AddMember(USquadMember* InMember, const bool bNotify = true);

	void RemoveMember(USquadMember* InMember, const bool bNotify = true);

	void TickSquadHandlers(const float DeltaTime);

	void BeginPlaySquadHandlers();

	void EndPlaySquadHandlers();

#if !UE_BUILD_SHIPPING
	void DrawDebugSquadTrace() const;
#endif
};
