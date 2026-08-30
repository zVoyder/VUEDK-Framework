// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameEffectsCue.h"
#include "Subsystems/WorldSubsystem.h"
#include "GameEffectsSubsystem.generated.h"

UCLASS()
class GAMEEFFECTSCUESYSTEM_API UGameEffectsSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

private:
	UPROPERTY()
	TArray<TObjectPtr<UGameEffectsCue>> ActiveCues;

public:
	void RegisterCue(UGameEffectsCue* Cue);

	void UnregisterCue(UGameEffectsCue* Cue);

private:
	void OnCueCompleted(UGameEffectsCue* GameEffectsCue);
};
