// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Saver.h"
#include "TransformSaver.generated.h"

UCLASS(Blueprintable, BlueprintType, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SAVESYSTEM_API UTransformSaver : public USaver
{
	GENERATED_BODY()

public:
	UTransformSaver();
	
	/**
	 * Called to prepare the save operation for the transform.
	 * @param SaveGameData - The save game data object.
	 * @param SlotInfoItem - The slot info item associated with the save.
	 * @param Instigator - The object that initiated the save.
	 */
	virtual void OnPrepareSave_Implementation(UDefaultSaveGame* SaveGameData, USlotInfoItem* SlotInfoItem, UObject* Instigator) override;
	
protected:
	/**
	 * Called when the component begins play. Used for initialization logic.
	 */
	virtual void BeginPlay() override;
	
	/**
	 * Called when a load operation is completed for the transform.
	 * @param SlotName - The name of the save slot.
	 * @param UserIndex - The user index for the load operation.
	 * @param LoadedData - The loaded save game data.
	 * @param Instigator - The object that initiated the load.
	 */
	virtual void OnLoadCompletedEvent_Implementation(const FString& SlotName, const int32 UserIndex, UDefaultSaveGame* LoadedData, UObject* Instigator) override;

	/**
	 * Gets the transform of the owner actor.
	 * @return The current transform of the owner actor.
	 */
	UFUNCTION(BlueprintCallable)
	FTransform GetOwnerTransform() const;
	
private:
	/**
	 * Sets the loaded transform to the owner actor from the loaded save data.
	 * @param LoadedData - The loaded save game data.
	 */
	void SetActorLoadedTransform(UDefaultSaveGame* LoadedData) const;
};
