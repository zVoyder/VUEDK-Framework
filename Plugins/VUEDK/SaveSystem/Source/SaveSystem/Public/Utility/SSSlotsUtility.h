// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "SaveManager.h"
#include "Data/Enums/SaveTypeFilter.h"
#include "Data/Structs/SlotInfoData.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "SSSlotsUtility.generated.h"

UCLASS()
class SAVESYSTEM_API USSSlotsUtility : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
private:
	static USaveManager* CurrentSaveManager;
	static FString SelectedSlotName;
	
public:
	/**
	 * Initializes the slots utility with the given save manager.
	 * @param SaveManager - The save manager to initialize with.
	 */
	static void Init(USaveManager* SaveManager);

	/**
	 * Clears the currently selected slot name.
	 */
	UFUNCTION(BlueprintCallable, Category = "Save System")
	static void ClearSelectedSlotName();
	
	/**
	 * Gets the currently selected slot name.
	 * @return The name of the selected slot.
	 */
	UFUNCTION(BlueprintPure, Category = "Save System")
	static FString GetSelectedSlotName();

	/**
	 * Checks if the currently selected slot is valid.
	 * @return True if the selected slot is valid, false otherwise.
	 */
	UFUNCTION(BlueprintPure, Category = "Save System")
	static bool IsSelectedSlotValid();

	/**
	 * Checks if a slot file exists for the given slot name.
	 * @param SlotName - The name of the slot to check.
	 * @return True if the slot file exists, false otherwise.
	 */
	UFUNCTION(BlueprintPure, Category = "Save System")
	static bool DoesSlotFileExist(const FString& SlotName);

	/**
	 * Checks if a shared slot file exists.
	 * @return True if a shared slot file exists, false otherwise.
	 */
	UFUNCTION(BlueprintPure, Category = "Save System")
	static bool DoesSharedSlotFileExist();

	/**
	 * Checks if any slot file exists, excluding shared slots if specified.
	 * @param bExcludeSharedSlot - If true, shared slots are excluded from the check.
	 * @return True if any slot file exists, false otherwise.
	 */
	UFUNCTION(BlueprintPure, Category = "Save System")
	static bool DoesAnySlotFileExist(const bool bExcludeSharedSlot = true);
	
	/**
	 * Tries to get all save file names.
	 * @param OutSaveFiles - Output parameter for the array of save file names.
	 * @param bWithExtension - If true, file extensions are included in the names.
	 * @return True if the save file names were retrieved successfully, false otherwise.
	 */
	UFUNCTION(BlueprintPure, Category = "Save System")
	static bool TryGetAllSaveFileNames(TArray<FString>& OutSaveFiles, bool bWithExtension = true);

	/**
	 * Gets the total number of save slots.
	 * @param bExcludeSharedSlot - If true, shared slots are excluded from the count.
	 * @return The total number of save slots.
	 */
	UFUNCTION(BlueprintPure, Category = "Save System")
	static int32 GetTotalSlots(const bool bExcludeSharedSlot = true);

	/**
	 * Gets the total number of auto save slots.
	 * @return The total number of auto save slots.
	 */
	UFUNCTION(BlueprintPure, Category = "Save System")
	static int32 GetTotalAutoSaveSlots();

	/**
	 * Gets the total number of manual save slots.
	 * @return The total number of manual save slots.
	 */
	UFUNCTION(BlueprintPure, Category = "Save System")
	static int32 GetTotalManualSaveSlots();

	/**
	 * Tries to get the shared slot info data.
	 * @param OutSlotInfo - Output parameter for the shared slot info data.
	 * @return True if the shared slot info was found, false otherwise.
	 */
	UFUNCTION(BlueprintPure, Category = "Save System")
	static bool TryGetSharedSlotInfoData(FSlotInfoData& OutSlotInfo);
	
	/**
	 * Tries to get slot infos of a specific type.
	 * @param OutSlotInfos - Output parameter for the slot info data array.
	 * @param Type - The type of slots to filter (default: All).
	 * @param bExcludeSharedSlot - If true, shared slots are excluded.
	 * @return True if slot infos were found, false otherwise.
	 */
	UFUNCTION(BlueprintPure, Category = "Save System")
	static bool TryGetSlotInfosOfType(TArray<FSlotInfoData>& OutSlotInfos, const ESaveTypeFilter Type = ESaveTypeFilter::All, const bool bExcludeSharedSlot = true);

	/**
	 * Tries to get the most recent slot info data.
	 * @param OutSlotData - Output parameter for the most recent slot info data.
	 * @param Type - The type of slots to filter (default: All).
	 * @return True if the most recent slot info was found, false otherwise.
	 */
	UFUNCTION(BlueprintPure, Category = "Save System")
	static bool TryGetMostRecentSlotInfoData(FSlotInfoData& OutSlotData, ESaveTypeFilter Type = ESaveTypeFilter::All);

	/**
	 * Tries to get the most ancient slot info data.
	 * @param OutSlotData - Output parameter for the most ancient slot info data.
	 * @param Type - The type of slots to filter (default: All).
	 * @return True if the most ancient slot info was found, false otherwise.
	 */
	UFUNCTION(BlueprintPure, Category = "Save System")
	static bool TryGetMostAncientSlotInfoData(FSlotInfoData& OutSlotData, ESaveTypeFilter Type = ESaveTypeFilter::All);
	
	/**
	 * Tries to select a save game slot by name.
	 * @param SlotName - The name of the slot to select.
	 * @return True if the slot was selected successfully, false otherwise.
	 */
	UFUNCTION(BlueprintCallable, Category = "Save System")
	static bool TrySelectSaveGameSlot(const FString SlotName);

	/**
	 * Tries to select the most recent save game slot.
	 * @return True if the most recent slot was selected successfully, false otherwise.
	 */
	UFUNCTION(BlueprintCallable, Category = "Save System")
	static bool TrySelectMostRecentSaveGame();

	/**
	 * Tries to select the most ancient save game slot.
	 * @return True if the most ancient slot was selected successfully, false otherwise.
	 */
	UFUNCTION(BlueprintPure, Category = "Save System")
	static bool TrySelectMostAncientSaveGame();
	
	/**
	 * Gets the played time of the most recent slot info.
	 * @return The played time as a float.
	 */
	UFUNCTION(BlueprintPure, Category = "Save System")
	static float GetMostRecentSlotInfoPlayedTime();

	/**
	 * Checks if the given slot info represents a shared slot.
	 * @param SlotInfo - The slot info data to check.
	 * @return True if the slot is shared, false otherwise.
	 */
	UFUNCTION(BlueprintPure, Category = "Save System")
	static bool IsSharedSlot(const FSlotInfoData& SlotInfo);
	
	/**
	 * Checks if the given slot name represents a shared slot.
	 * @param SlotName - The slot name to check.
	 * @return True if the slot name is shared, false otherwise.
	 */
	static bool IsSharedSlotName(const FString& SlotName);
	
	/**
	 * Checks if the given slot name is valid.
	 * @param SlotName - The slot name to validate.
	 * @return True if the slot name is valid, false otherwise.
	 */
	static bool IsSlotNameValid(const FString& SlotName);
};
