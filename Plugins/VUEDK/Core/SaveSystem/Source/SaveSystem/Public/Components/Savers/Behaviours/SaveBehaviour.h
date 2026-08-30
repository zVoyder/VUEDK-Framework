// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Data/SaveData/SaveDataBase.h"
#include "Interfaces/Saveable.h"
#include "UObject/Object.h"
#include "SaveBehaviour.generated.h"

UCLASS(Blueprintable, BlueprintType, EditInlineNew)
class SAVESYSTEM_API USaveBehaviour : public UObject, public ISaveable
{
	GENERATED_BODY()

	friend class USaver;

public:
	// If the SaveClass CDO fails to save, clear DerivedDataCache, Saved, Intermediate, and Binaries folders,
	// regenerate project files, recompile, and restart the editor.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<USaveDataBase> SaveClass;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ToolTip = "If true, the SaveData instance will not be created every time a save/load is performed, but will be preserved between saves/loads."))
	bool bPreserveSaveDataInstance = false;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FGuid UniqueBehaviourID;

private:
	UPROPERTY()
	USaveDataBase* CachedSaveData = nullptr;
	UPROPERTY()
	USaver* CachedOwnerSaver = nullptr;

public:
	USaveBehaviour();

	void Init(USaver* OwnerSaver);

	virtual void BeginPlay();

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason);
	
	virtual USaveDataBase* CreateSaveDataInstanceNative();
	
	virtual USaveDataBase* CreateSaveDataInstance_Implementation() override;

	virtual void PrepareForSerializationNative(USaveDataBase* SaveData);

	virtual void PrepareForSerialization_Implementation(USaveDataBase* SaveData) override;

	virtual void PrepareForDeserializationNative(USaveDataBase* SaveData);

	virtual void PrepareForDeserialization_Implementation(USaveDataBase* SaveData) override;

	virtual bool Save_Implementation(USaveDataBase* SaveData) override;

	virtual bool Load_Implementation(USaveDataBase* SaveData) override;

	UFUNCTION(BlueprintPure)
	USaveDataBase* GetSaveDataInstance();

	UFUNCTION(BlueprintPure)
	FName GetCompositeSaveBehaviourID() const;

	UFUNCTION(BlueprintPure)
	virtual bool Check() const;

#if WITH_EDITOR
	virtual bool ImplementsGetWorld() const override;
#endif

	virtual UWorld* GetWorld() const override;

protected:
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "BeginPlay"))
	void ReceiveBeginPlay();

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "EndPlay"))
	void ReceiveEndPlay(const EEndPlayReason::Type EndPlayReason);
	
	UFUNCTION(BlueprintPure)
	USaver* GetOwnerSaver();

	UFUNCTION(BlueprintPure)
	AActor* GetOwnerActor();

private:
	void CacheSaveDataInstanceIfNeeded();
};
