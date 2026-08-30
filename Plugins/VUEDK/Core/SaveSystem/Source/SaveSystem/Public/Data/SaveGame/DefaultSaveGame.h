// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "DefaultSaveGame.generated.h"

USTRUCT()
struct FSerializedObject
{
	GENERATED_BODY()
	
	UPROPERTY(SaveGame)
	TArray<uint8> Bytes;
};

UCLASS(Abstract, BlueprintType, Blueprintable)
class SAVESYSTEM_API UDefaultSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FName SlotNameKey;
	UPROPERTY()
	TMap<FName, FSerializedObject> SavedObjects;
	UPROPERTY()
	TArray<FName> DestroyedActors;
	
	UDefaultSaveGame(): SlotNameKey("")
	{
	}

	void SetSlotNameKey(const FName& Key)
	{
		SlotNameKey = Key;
	}
};
