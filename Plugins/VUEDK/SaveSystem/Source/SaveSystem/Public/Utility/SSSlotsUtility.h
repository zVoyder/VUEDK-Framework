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
	static void Init(USaveManager* SaveManager);

	UFUNCTION(BlueprintCallable, Category = "Save System")
	static void ClearSelectedSlotName();
	
	UFUNCTION(BlueprintPure, Category = "Save System")
	static FString GetSelectedSlotName();

	UFUNCTION(BlueprintPure, Category = "Save System")
	static bool IsSelectedSlotValid();

	UFUNCTION(BlueprintPure, Category = "Save System")
	static bool DoesSlotFileExist(const FString& SlotName);

	UFUNCTION(BlueprintPure, Category = "Save System")
	static bool DoesSharedSlotFileExist();

	UFUNCTION(BlueprintPure, Category = "Save System")
	static bool DoesAnySlotFileExist(const bool bExcludeSharedSlot = true);
	
	UFUNCTION(BlueprintPure, Category = "Save System")
	static bool TryGetAllSaveFileNames(TArray<FString>& OutSaveFiles, bool bWithExtension = true);

	UFUNCTION(BlueprintPure, Category = "Save System")
	static int32 GetTotalSlots(const bool bExcludeSharedSlot = true);

	UFUNCTION(BlueprintPure, Category = "Save System")
	static int32 GetTotalAutoSaveSlots();

	UFUNCTION(BlueprintPure, Category = "Save System")
	static int32 GetTotalManualSaveSlots();

	UFUNCTION(BlueprintPure, Category = "Save System")
	static bool TryGetSharedSlotInfoData(FSlotInfoData& OutSlotInfo);
	
	UFUNCTION(BlueprintPure, Category = "Save System")
	static bool TryGetSlotInfosOfType(TArray<FSlotInfoData>& OutSlotInfos, const ESaveTypeFilter Type = ESaveTypeFilter::All, const bool bExcludeSharedSlot = true);

	UFUNCTION(BlueprintPure, Category = "Save System")
	static bool TryGetMostRecentSlotInfoData(FSlotInfoData& OutSlotData, ESaveTypeFilter Type = ESaveTypeFilter::All);

	UFUNCTION(BlueprintPure, Category = "Save System")
	static bool TryGetMostAncientSlotInfoData(FSlotInfoData& OutSlotData, ESaveTypeFilter Type = ESaveTypeFilter::All);
	
	UFUNCTION(BlueprintCallable, Category = "Save System")
	static bool TrySelectSaveGameSlot(const FString SlotName);

	UFUNCTION(BlueprintCallable, Category = "Save System")
	static bool TrySelectMostRecentSaveGame();

	UFUNCTION(BlueprintPure, Category = "Save System")
	static bool TrySelectMostAncientSaveGame();
	
	UFUNCTION(BlueprintPure, Category = "Save System")
	static float GetMostRecentSlotInfoPlayedTime();

	UFUNCTION(BlueprintPure, Category = "Save System")
	static bool IsSharedSlot(const FSlotInfoData& SlotInfo);
	
	static bool IsSharedSlotName(const FString& SlotName);
	
	static bool IsSlotNameValid(const FString& SlotName);
};
