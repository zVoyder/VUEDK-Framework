// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Objects/SmartRespawnBehaviours/Base/SmartRespawnBehaviour.h"
#include "Data/SmartRespawnSnapshot.h"
#include "SmartRespawn.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SMARTRESPAWNSYSTEM_API USmartRespawn : public USceneComponent
{
	GENERATED_BODY()

	friend class USmartRespawnBehaviour;
	
public:
	UPROPERTY(BlueprintAssignable, Category = Events)
	FOnRespawnStarted OnRespawnStarted;
	UPROPERTY(BlueprintAssignable, Category = Events)
	FOnRespawnFinished OnRespawnFinished;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Smart Respawn")
	FGameplayTag RespawnTag;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Smart Respawn")
	int32 Priority = 0;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Smart Respawn")
	bool bUseCustomRespawn = false;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Instanced, meta = (EditCondition="bUseCustomRespawn", EditConditionHides), Category = "Smart Respawn")
	TObjectPtr<USmartRespawnBehaviour> CustomRespawnBehaviour;

public:
	USmartRespawn();

	FSmartRespawnSnapshot CreateRespawnSnapshot();

	UFUNCTION(BlueprintCallable)
	bool RegisterRespawn(const APlayerState* PlayerState, const bool bOverrideExisting = true);

	UFUNCTION(BlueprintCallable, BlueprintPure = false)
	bool UnregisterRespawn(const APlayerState* PlayerState) const;

private:
	void NotifyStartRespawn(const APlayerController* PlayerController, const FSmartRespawnSnapshot& RespawnSnapshot) const;

	void NotifyFinishRespawn(const APlayerController* PlayerController, const FSmartRespawnSnapshot& RespawnSnapshot, const bool bSuccess) const;
};
