// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "SmartRespawnSubsystem.h"
#include "UObject/Object.h"
#include "SmartRespawnBehaviour.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(
	FOnRespawnStarted,
	const APlayerController*, PlayerController,
	FGameplayTag, RespawnTag,
	FSmartRespawnSnapshot, RespawnSnapshot
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(
	FOnRespawnFinished,
	const APlayerController*, PlayerController,
	FGameplayTag, RespawnTag,
	FSmartRespawnSnapshot, RespawnSnapshot,
	bool, bSuccess
);

UCLASS(Abstract, Blueprintable, EditInlineNew, DefaultToInstanced)
class SMARTRESPAWNSYSTEM_API USmartRespawnBehaviour : public USmartRespawnObjectBase
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable, Category = Events)
	FOnRespawnStarted OnRespawnStarted;
	UPROPERTY(BlueprintAssignable, Category = Events)
	FOnRespawnFinished OnRespawnFinished;

private:
	UPROPERTY()
	APlayerController* PlayerController;
	UPROPERTY()
	FGameplayTag RespawnTag;
	UPROPERTY()
	FSmartRespawnSnapshot RespawnSnapshot;
	UPROPERTY()
	USmartRespawn* SmartRespawn;
	UPROPERTY()
	AActor* Owner;

public:
	void StartRespawn(APlayerController* InPlayerController, FGameplayTag InRespawnTag, const FSmartRespawnSnapshot& InRespawnSnapshot);

	UFUNCTION(BlueprintPure)
	USmartRespawn* GetSmartRespawn() const;
	
	UFUNCTION(BlueprintPure)
	AActor* GetOwner() const;
	
protected:
	UFUNCTION(BlueprintNativeEvent)
	void OnRespawnStart(const APlayerController* InPlayerController, const FGameplayTag& InRespawnTag, const FSmartRespawnSnapshot& InRespawnSnapshot);

	UFUNCTION(BlueprintNativeEvent)
	void OnRespawnFinish(const APlayerController* InPlayerController, const FGameplayTag& InRespawnTag, const FSmartRespawnSnapshot& InRespawnSnapshot, const bool bSuccess);
	
	UFUNCTION(BlueprintCallable, BlueprintPure = false, meta = (AdvancedDisplay = 1))
	void ApplyRespawnSnapshot(const FSmartRespawnSnapshot& InRespawnSnapshot, const bool bFinish = false, const bool bSuccess = true);

	UFUNCTION(BlueprintCallable)
	void FinishExecute(const bool bSuccess = true);

private:
	void NotifyStartRespawn();
	
	void NotifyFinishRespawn(bool bSuccess);
};
