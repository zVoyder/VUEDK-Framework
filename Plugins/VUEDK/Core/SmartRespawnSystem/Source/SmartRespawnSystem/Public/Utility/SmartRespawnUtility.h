// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "SmartRespawnSubsystem.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "SmartRespawnUtility.generated.h"

UCLASS()
class SMARTRESPAWNSYSTEM_API USmartRespawnUtility : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

private:
	static TWeakObjectPtr<USmartRespawnSubsystem> SmartRespawnSubsystem;
	
public:
	static void Init(USmartRespawnSubsystem* InSmartRespawnSubsystem);

	UFUNCTION(BlueprintCallable, Category = "Smart Respawn System")
	static void SaveRegister();

	UFUNCTION(BlueprintCallable, Category = "Smart Respawn System")
	static void LoadRegister();

	UFUNCTION(BlueprintCallable, Category = "Smart Respawn System")
	static void SetRespawnsRegister(const FSmartRespawnRegister& NewRegister);
	
	UFUNCTION(BlueprintPure, Category = "Smart Respawn System")
	static const FSmartRespawnRegister& GetRespawnsRegister();

	UFUNCTION(BlueprintCallable, meta = (AdvancedDisplay = 2), Category = "Smart Respawn System")
	static bool RegisterPlayerRespawn(const APlayerState* Player, const FGameplayTag RespawnTag, FSmartRespawnSnapshot RespawnSnapshot, const bool bOverrideExisting = true);

	UFUNCTION(BlueprintCallable, Category = "Smart Respawn System")
	static bool UnregisterPlayerRespawn(const APlayerState* Player,  FGameplayTag RespawnTag);

	UFUNCTION(BlueprintPure, Category = "Smart Respawn System")
	static bool FindPlayerRespawn(const APlayerState* Player, const TSubclassOf<USmartRespawnSelectorBase> SelectorClass, FGameplayTag& OutFoundRespawnTag, FSmartRespawnSnapshot& OutRespawnSnapshot);
	
	UFUNCTION(BlueprintPure, Category = "Smart Respawn System")
	static bool GetPlayerRespawn(const APlayerState* Player, const FGameplayTag RespawnTag, FSmartRespawnSnapshot& OutRespawnSnapshot);

	UFUNCTION(BlueprintPure, Category = "Smart Respawn System")
	static bool GetPlayerRespawns(const APlayerState* Player, TMap<FGameplayTag, FSmartRespawnSnapshot>& OutRespawns);

	UFUNCTION(BlueprintCallable, Category = "Smart Respawn System")
	static bool ClearPlayerRespawns(const APlayerState* Player);

	UFUNCTION(BlueprintCallable, Category = "Smart Respawn System")
	static void ClearRegister();

	UFUNCTION(BlueprintCallable, meta = (AdvancedDisplay = 1), Category = "Smart Respawn System")
	static bool RespawnPlayer(APlayerController* PlayerController, const TSubclassOf<USmartRespawnSelectorBase> SelectorClass = nullptr);

	UFUNCTION(BlueprintCallable, Category = "Smart Respawn System")
	static bool RespawnPlayerAt(APlayerController* PlayerController, const FGameplayTag RespawnTag);

	static void NotifyRespawnStart(const APlayerController* PlayerController, const FGameplayTag& RespawnTag, const FSmartRespawnSnapshot& RespawnSnapshot, USmartRespawnBehaviour* RespawnBehaviour);

	static void NotifyRespawnFinish(const APlayerController* PlayerController, const FGameplayTag& RespawnTag, const FSmartRespawnSnapshot& RespawnSnapshot, USmartRespawnBehaviour* RespawnBehaviour, const bool bSuccess);
	
private:
	static bool Check();
};
