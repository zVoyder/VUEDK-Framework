// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "SquadSubsystem.h"
#include "SquadUtility.generated.h"

UCLASS()
class SQUADSYSTEM_API USquadUtility : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

private:
	static TWeakObjectPtr<USquadSubsystem> SquadSubsystem;

public:
	static void Init(USquadSubsystem* Subsystem);

	static bool ResolveJoin(USquadMember* Requester, USquadMember* Receiver, const ESquadResolution Resolution);

	static bool ResolveLeave(USquadMember* MemberToLeave, const ESquadResolution Resolution);

	/**
	 * @brief Simply creates a new squad with the given members and SquadID.
	 * @param Members Members to be added to the new squad.
	 * @param SquadID Unique identifier for the squad.
	 * @return The created squad or nullptr if creation failed.
	 */
	UFUNCTION(BlueprintCallable, Category = "Squad System")
	static USquad* SimpleCreateSquad(const TArray<USquadMember*>& Members, const FName SquadID);

	/**
	 * @brief Removes the squad with the specified SquadID.
	 * @param SquadID Unique identifier of the squad to be deleted.
	 */
	UFUNCTION(BlueprintCallable, Category = "Squad System")
	static void RemoveSquad(const FName SquadID);

	/**
	 * @brief Clears all squads from the system.
	 */
	UFUNCTION(BlueprintCallable, Category = "Squad System")
	static void ClearSquads();

	UFUNCTION(BlueprintPure, Category = "Squad System")
	static int32 GetMaxMembersPerSquad();

	UFUNCTION(BlueprintPure, Category = "Squad System")
	static USquad* FindSquad(const FName SquadID);

	UFUNCTION(BlueprintPure, Category = "Squad System")
	static FSquadInfo GetSquadInfo(const FName SquadID);

private:
	static bool Check();
};
