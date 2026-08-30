// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Data/SmartRespawnRegister.h"
#include "Data/SmartRespawnSnapshot.h"
#include "Objects/RegisterSavers/Base/SmartRespawnRegisterSaverBase.h"
#include "Subsystems/WorldSubsystem.h"
#include "SmartRespawnSubsystem.generated.h"

class USmartRespawnSelectorBase;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(
	FOnRespawnRegistered,
	const APlayerState*, Player,
	const FGameplayTag&, RespawnTag,
	const FSmartRespawnSnapshot&, RespawnSnapshot
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(
	FOnRespawnUnregistered,
	const APlayerState*, Player,
	const FGameplayTag&, RespawnTag,
	const FSmartRespawnSnapshot&, RespawnSnapshot
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
	FOnRespawnRequested,
	bool, bSuccess
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(
	FOnRespawnBehaviourStarted,
	const APlayerController*, InPlayerController,
	const FGameplayTag&, InRespawnTag,
	const FSmartRespawnSnapshot& ,InRespawnSnapshot,
	USmartRespawnBehaviour*, RespawnBehaviour
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(
	FOnRespawnBehaviourFinished,
	const APlayerController*, InPlayerController,
	const FGameplayTag&, InRespawnTag,
	const FSmartRespawnSnapshot&, InRespawnSnapshot,
	bool, bSuccess
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
	FOnRestoreFinished,
	bool, bSuccess
);

UCLASS()
class SMARTRESPAWNSYSTEM_API USmartRespawnSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable, Category = Events)
	FOnRespawnRegistered OnRespawnRegistered;
	UPROPERTY(BlueprintAssignable, Category = Events)
	FOnRespawnUnregistered OnRespawnUnregistered;
	UPROPERTY(BlueprintAssignable, Category = Events)
	FOnRespawnBehaviourStarted OnRespawnStarted;
	UPROPERTY(BlueprintAssignable, Category = Events)
	FOnRespawnBehaviourFinished OnRespawnFinished;
	UPROPERTY(BlueprintAssignable, Category = Events)
	FOnRespawnRequested OnRespawnRequested;
	UPROPERTY(BlueprintAssignable, Category = Events)
	FOnRespawnRequested OnPostLoginRespawnRequested;
	UPROPERTY(BlueprintAssignable, Category = Events)
	FOnRestoreFinished OnRestoreFinished;
	UPROPERTY(BlueprintAssignable, Category = Events)
	FOnRestoreFinished OnPostLoginRestoreFinished;
	
private:
	UPROPERTY()
	FSmartRespawnRegister Register;
	UPROPERTY()
	USmartRespawnRegisterSaverBase* RegisterSaver;
	UPROPERTY()
	APlayerController* RespawningPlayer;

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	virtual void Deinitialize() override;
	
	void SetRegister(const FSmartRespawnRegister& NewRegister);

	const FSmartRespawnRegister& GetRegister() const;

	bool RegisterPlayerRespawn(const APlayerState* Player, const FGameplayTag& RespawnTag, FSmartRespawnSnapshot& RespawnSnapshot, const bool bOverrideExisting = true);

	bool UnregisterPlayerRespawn(const APlayerState* Player, const FGameplayTag& RespawnTag);

	bool FindPlayerRespawn(const APlayerState* Player, const TSubclassOf<USmartRespawnSelectorBase> SelectorClass, FGameplayTag& OutRespawnTag, FSmartRespawnSnapshot& OutRespawnSnapshot);
	
	bool GetPlayerRespawn(const APlayerState* Player, const FGameplayTag& RespawnTag, FSmartRespawnSnapshot& OutRespawnSnapshot) const;

	bool GetPlayerRespawns(const APlayerState* Player, TMap<FGameplayTag, FSmartRespawnSnapshot>& OutRespawns) const;

	bool ClearPlayerRespawns(const APlayerState* Player);

	void ClearRegister();

	void SaveRegister() const;

	void RestoreRegister();

	bool RequestRespawnPlayer(APlayerController* PlayerController, TSubclassOf<USmartRespawnSelectorBase> InSelectorClass = nullptr);

	bool RequestRespawnPlayerAt(APlayerController* PlayerController, const FGameplayTag& RespawnTag);
	
	void NotifyRespawnStart(const APlayerController* PlayerController, const FGameplayTag& RespawnTag, const FSmartRespawnSnapshot& RespawnSnapshot, USmartRespawnBehaviour* RespawnBehaviour) const;

	void NotifyRespawnFinish(const APlayerController* PlayerController, const FGameplayTag& RespawnTag, const FSmartRespawnSnapshot& RespawnSnapshot, const bool bSuccess) const;

private:
	bool HandleRespawnPlayer(APlayerController* PlayerController, TSubclassOf<USmartRespawnSelectorBase> InSelectorClass);

	bool HandleRespawnPlayerAt(APlayerController* PlayerController, const FGameplayTag& RespawnTag);

	bool StartStreamingSourceRespawn(APlayerController* PlayerController, const FGameplayTag RespawnTag, const FSmartRespawnSnapshot& RespawnSnapshot, FName PlayerId);
	
	void OnPostLogin(AGameModeBase* GameModeBase, APlayerController* PlayerController);

	UFUNCTION()
	void OnPossessedPawnChanged(APawn* OldPawn, APawn* NewPawn);

	void InitStartRespawnPlayer(APlayerController* PlayerController);

	void NewEntrySave() const;

	void RegisterFinishRestore(const bool bSuccess) const;

	void PostLoginRegisterFinishRestore(const bool bSuccess);

	void PostLoginRespawnPlayer();

	void PostLoginRestore();

	bool PostLoginShouldRespawnPlayer() const;

	static void EnableInput(APlayerController* PlayerController);
	
	static void DisableInput(APlayerController* PlayerController);
	
	static FName GetPlayerId(const APlayerState* Player);
	
	static bool IsPlayFromHere(APlayerController* PlayerController);

	void OnRespawnStreamingSourceCompleted(APlayerController* PlayerController, const FGameplayTag& RespawnTag, const FSmartRespawnSnapshot& RespawnSnapshot);
};
