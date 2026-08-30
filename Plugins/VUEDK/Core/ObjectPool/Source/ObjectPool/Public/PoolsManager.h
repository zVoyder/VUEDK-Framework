// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ActorPool.h"
#include "GameplayTagContainer.h"
#include "PoolsManager.generated.h"

UCLASS(NotBlueprintable, BlueprintType)
class OBJECTPOOL_API UPoolsManager : public UObject
{
	GENERATED_BODY()

private:
	TArray<FActorPoolData> PoolsData;
	UPROPERTY()
	TArray<UActorPool*> ActorPools;
	UPROPERTY()
	UWorld* WorldContextObject;

public:
	UPoolsManager();

	void Init(UWorld* InWorldContextObject, const TArray<FActorPoolData>& InPoolsData);
	
	UActorPool* GetPoolByTag(FGameplayTag PoolTag) const;

	UActorPool* GetPoolByClass(const TSubclassOf<AActor> ActorClass) const;

protected:
	virtual void BeginDestroy() override;
	
private:
	void InitPools();

	void DestroyPools();

	bool IsValidPool(const FActorPoolData& PoolData) const;
	
	bool Check() const;
};
