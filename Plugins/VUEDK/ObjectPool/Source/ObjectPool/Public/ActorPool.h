// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Data/ActorPoolData.h"
#include "UObject/Object.h"
#include "ActorPool.generated.h"

UCLASS(NotBlueprintable, BlueprintType)
class OBJECTPOOL_API UActorPool : public UObject
{
	GENERATED_BODY()

private:
	UPROPERTY()
	TArray<AActor*> AvailableActors;
	UPROPERTY()
	TArray<AActor*> UsedActors;
	UPROPERTY()
	TSubclassOf<AActor> PooledActorClass;
	UPROPERTY()
	TObjectPtr<UWorld> WorldContextObject;
	FActorPoolData PoolData;

public:
	void Init(const TObjectPtr<UWorld>& InWorldContextObject, const FActorPoolData InPoolData);

	UFUNCTION(BlueprintCallable)
	void DestroyPool();

	UFUNCTION(BlueprintPure)
	TSubclassOf<AActor> GetActorClass() const;

	UFUNCTION(BlueprintPure)
	AActor* AcquireActor();

	UFUNCTION(BlueprintCallable)
	void ReleaseActor(AActor* InActor);

	void SetActorClass(const TSubclassOf<AActor>& InActorClass);

	virtual UWorld* GetWorld() const override;

private:
	void SpawnInstances(const int32 InCount);

	void SpawnInstance();

	void PushActor(AActor* Actor);

	AActor* PopActor();

	bool IsFull() const;

	bool IsEmpty() const;

	bool Check() const;

	void SetActorEnabled(AActor* Actor, bool bIsEnabled) const;

	static void ClearActor(AActor* Actor);
};
