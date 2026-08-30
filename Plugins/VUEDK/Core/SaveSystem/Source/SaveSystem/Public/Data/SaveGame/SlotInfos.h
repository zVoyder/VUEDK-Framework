// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "Data/Structs/SlotInfoData.h"
#include "SlotInfos.generated.h"

UCLASS()
class SAVESYSTEM_API USlotInfos : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<FName, FSlotInfoData> SlotInfos = TMap<FName, FSlotInfoData>();
};
