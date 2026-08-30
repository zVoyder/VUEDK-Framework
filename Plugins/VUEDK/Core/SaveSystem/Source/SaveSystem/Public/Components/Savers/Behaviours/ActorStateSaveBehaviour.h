// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "SaveBehaviour.h"
#include "ActorStateSaveBehaviour.generated.h"

UCLASS()
class SAVESYSTEM_API UActorStateSaveBehaviour : public USaveBehaviour
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ActorSaveBehaviour|State")
	bool bMarkOnDestroy = true;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ActorSaveBehaviour|Transform")
	bool bSaveLocation = true;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ActorSaveBehaviour|Transform")
	bool bSaveRotation = true;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ActorSaveBehaviour|Transform")
	bool bSaveScale = true;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ActorSaveBehaviour|Physics")
	bool bSaveLinearVelocity = false;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ActorSaveBehaviour|Physics")
	bool bSaveAngularVelocity = false;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ActorSaveBehaviour|Physics")
	bool bSaveMass = false;

public:
	virtual USaveDataBase* CreateSaveDataInstanceNative() override;

	virtual bool Save_Implementation(USaveDataBase* SaveData) override;

	virtual bool Load_Implementation(USaveDataBase* SaveData) override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

protected:
	/**
	 * @brief Marks the owning Actor as destroyed in the current SaveGame. This will cause the Actor to be destroyed when loading.
	 */
	UFUNCTION(BlueprintCallable)
	void MarkAsDestroyed() const;

	/**
	 * @brief Marks the owning Actor as not destroyed in the current SaveGame.
	 * This will prevent the Actor from being destroyed when loading.
	 */
	UFUNCTION(BlueprintCallable)
	void MarkAsNotDestroyed() const;
};
