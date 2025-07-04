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
	TMap<FGameplayTag, UActorPool*> ActorPools;
	UPROPERTY()
	UWorld* WorldContextObject;

public:
	UPoolsManager();

	void Init(UWorld* InWorldContextObject, const TArray<FActorPoolData> InPoolsData);
	
	UActorPool* GetPool(FGameplayTag PoolTag) const;

	UActorPool* GetPoolByClass(const UClass* ActorClass) const;

protected:
	virtual void BeginDestroy() override;
	
private:
	void InitPools();

	void DestroyPools();

	bool Check() const;
};
