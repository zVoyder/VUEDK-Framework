// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "StatOperation.h"
#include "BridgeStatOperation.h"
#include "Components/ActorComponent.h"
#include "Containers/CoreStatsContainer.h"
#include "Containers/SpecialStatsContainer.h"
#include "Data/SpecialStatData.h"
#include "Interfaces/Saveable.h"
#include "StatsBridgeBase.generated.h"

DEFINE_LOG_CATEGORY_STATIC(LogStatsSystem, Log, All);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
	FOnCalculatedCoreStatsValues,
	UStatsBridgeBase*, StatsBridge
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
	FOnCalculatedFullStatsValues,
	UStatsBridgeBase*, StatsBridge
);

UCLASS(Abstract, Blueprintable, BlueprintType, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class RPGSYSTEM_API UStatsBridgeBase : public UActorComponent, public ISaveable
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable, Category = Events)
	FOnCalculatedCoreStatsValues OnCalculatedCoreStatsValues;
	UPROPERTY(BlueprintAssignable, Category = Events)
	FOnCalculatedFullStatsValues OnCalculatedFullStatsValues;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSet<USpecialStatData*> SpecialStats;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSet<UCoreStatData*> CoreStats;
	UPROPERTY(BlueprintReadOnly)
	USpecialStatsContainer* SpecialStatsContainer;
	UPROPERTY(BlueprintReadOnly)
	UCoreStatsContainer* CoreStatsContainer;

private:
	UPROPERTY()
	UCoreStatsContainer* FullStatsContainer;

public:
	UStatsBridgeBase();
	
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UFUNCTION(BlueprintPure)
	virtual USaveData* CreateSaveData() override;

	UFUNCTION(BlueprintCallable)
	virtual bool LoadSaveData(USaveData* SavedData) override;

	/**
	 * Gets the special stat data by its unique identifier.
	 * @param SpecialStatID - The unique identifier of the special stat.
	 * @return Pointer to the USpecialStatData if found, nullptr otherwise.
	 */
	USpecialStatData* GetSpecialStatByID(const FGuid SpecialStatID) const;

	/**
	 * Gets the core stat data by its unique identifier.
	 * @param CoreStatID - The unique identifier of the core stat.
	 * @return Pointer to the UCoreStatData if found, nullptr otherwise.
	 */
	UCoreStatData* GetCoreStatByID(const FGuid CoreStatID) const;

	/**
	 * Gets the special stat data by its code name.
	 * @param SpecialStatCodeName - The code name of the special stat.
	 * @return Pointer to the USpecialStatData if found, nullptr otherwise.
	 */
	UFUNCTION(BlueprintPure)
	USpecialStatData* GetSpecialStatByCodeName(const FString& SpecialStatCodeName) const;

	/**
	 * Gets the core stat data by its code name.
	 * @param CoreStatCodeName - The code name of the core stat.
	 * @return Pointer to the UCoreStatData if found, nullptr otherwise.
	 */
	UFUNCTION(BlueprintPure)
	UCoreStatData* GetCoreStatByCodeName(const FString& CoreStatCodeName) const;
	
	/**
	 * Gets a map of all core stat data and their full values.
	 * @return Map of UCoreStatData pointers to their float values.
	 */
	UFUNCTION(BlueprintPure)
	TMap<UCoreStatData*, float> GetFullStatsValues() const;

	/**
	 * Gets the full stats container for this bridge.
	 * @return Pointer to the UCoreStatsContainer containing all full stats.
	 */
	UFUNCTION(BlueprintPure)
	UCoreStatsContainer* GetFullStatsContainer() const;

	/**
	 * Gets the value of a specific stat as a float.
	 * @param Stat - The stat data object to query.
	 * @return The value of the stat as a float.
	 */
	UFUNCTION(BlueprintPure)
	float GetFullStatValueAsFloat(const UStatDataBase* Stat) const;

	/**
	 * Gets the value of a specific stat as an integer.
	 * @param Stat - The stat data object to query.
	 * @return The value of the stat as an integer.
	 */
	UFUNCTION(BlueprintPure)
	int32 GetFullStatValueAsInt(const UStatDataBase* Stat) const;

	/**
	 * Gets the value of a specific stat as a string.
	 * @param Stat - The stat data object to query.
	 * @return The value of the stat as a string.
	 */
	UFUNCTION(BlueprintPure)
	FString GetFullStatValueAsString(const UStatDataBase* Stat) const;

	/**
	 * Gets the number of full stats in the bridge.
	 * @return The number of full stats.
	 */
	UFUNCTION(BlueprintPure)
	int32 GetFullStatsLength() const;
	
	/**
	 * Calculates the value of a core stat based on a special stat and a bridge operation.
	 * @param InSpecialStat - The special stat to use for the calculation.
	 * @param CoreStatData - The core stat data to modify.
	 * @param OperationClass - The class of the bridge operation to use for the calculation.
	 */
	UFUNCTION(BlueprintCallable)
	void CalculateCoreStatValueWithSpecialStat(USpecialStatData* InSpecialStat, UCoreStatData* CoreStatData, const TSubclassOf<UBridgeStatOperation> OperationClass);

	UFUNCTION(BlueprintCallable)
	void CalculateAllStatsValues();

protected:
	virtual void BeginPlay() override;

	/**
	 * Sets the value of a full stat in the full stats container.
	 * @param Stat - The stat data object to set.
	 * @param Value - The value to assign to the stat.
	 */
	UFUNCTION(BlueprintCallable)
	void SetFullStatValue(UStatDataBase* Stat, float Value) const;

	/**
	 * Modifies the value of a full stat in the full stats container by adding the specified value.
	 * @param Stat - The stat data object to modify.
	 * @param Value - The value to add to the stat.
	 */
	UFUNCTION(BlueprintCallable)
	void ModifyFullStatValue(UStatDataBase* Stat, float Value) const;

	/**
	 * Calculates the values of all core stats in the bridge.
	 * Called internally during stat calculations.
	 */
	UFUNCTION()
	void CalculateCoreStatsValues();

	/**
	 * Calculates the values of all full stats in the bridge.
	 * Called internally during stat calculations.
	 */
	UFUNCTION()
	void CalculateFullStatsValues();

	/**
	 * Native implementation for calculating core stats values. Can be overridden in subclasses.
	 */
	virtual void NativeOnCalculateCoreStatsValues();
	
	/**
	 * Blueprint event called when core stats values are calculated. Can be implemented in Blueprints.
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnCalculateCoreStatsValues();
	
	/**
	 * Native implementation for calculating full stats values. Can be overridden in subclasses.
	 */
	virtual void NativeOnCalculateFullStatsValues();
	
	/**
	 * Blueprint event called when full stats values are calculated. Can be implemented in Blueprints.
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnCalculateFullStatsValues();

private:
	/**
	 * Creates and initializes the stats containers used by this bridge.
	 */
	void CreateStatsContainers();
};
