// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "SaveBehaviour.h"
#include "SaveComponentBehaviour.generated.h"

UCLASS()
class SAVESYSTEM_API USaveComponentBehaviour : public USaveBehaviour
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UActorComponent> ComponentClass;

private:
	UPROPERTY()
	UActorComponent* CachedComponent = nullptr;

public:
	virtual void BeginPlay() override;
	
	virtual bool Save_Implementation(USaveDataBase* SaveData) override;

	virtual bool Load_Implementation(USaveDataBase* SaveData) override;
	
private:
	UActorComponent* FindComponent();
};
