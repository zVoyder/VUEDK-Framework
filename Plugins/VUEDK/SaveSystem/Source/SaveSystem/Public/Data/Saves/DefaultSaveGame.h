// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "DefaultSaveGame.generated.h"

UCLASS(BlueprintType, Blueprintable)
class SAVESYSTEM_API UDefaultSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FName SlotNameKey;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TMap<FName, FTransform> ActorTransforms;

	UDefaultSaveGame(): SlotNameKey(""),
	                    ActorTransforms(TMap<FName, FTransform>())
	{
	}

	void SetSlotNameKey(const FName& Key)
	{
		SlotNameKey = Key;
	}
};
