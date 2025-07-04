// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Effects/GameEffectBase.h"
#include "UObject/Object.h"
#include "GameEffectsCue.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(
	FOnCueStarted
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(
	FOnCueStopped
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(
	FOnCueResumed
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(
	FOnCueFinished
);

UCLASS(NotBlueprintable, BlueprintType, EditInlineNew)
class GAMEEFFECTSCUESYSTEM_API UGameEffectsCue : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable, Category = Events)
	FOnCueStarted OnCueStarted;
	UPROPERTY(BlueprintAssignable, Category = Events)
	FOnCueStopped OnCueStopped;
	UPROPERTY(BlueprintAssignable, Category = Events)
	FOnCueResumed OnCueResumed;
	UPROPERTY(BlueprintAssignable, Category = Events)
	FOnCueFinished OnCueFinished;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GameEffects")
	int32 EffectStartIndex = 0;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Instanced, Category = "GameEffects")
	TArray<UGameEffectBase*> Effects;

private:
	bool bIsPlaying = false;
	int32 CurrentIndex = 0;

public:
	/**
	 * Initializes the cue with the given instigator.
	 * @param Instigator The actor that instigates the cue.
	 */
	void Init(AActor* Instigator);

	/**
	 * Starts the cue sequence.
	 */
	UFUNCTION(BlueprintCallable)
	void StartCue();

	/**
	 * Stops the cue sequence.
	 */
	UFUNCTION(BlueprintCallable)
	void StopCue();

	/**
	 * Resumes the cue sequence from the current effect.
	 */
	UFUNCTION(BlueprintCallable)
	void ResumeCue();

	/**
	 * Interrupts the cue sequence immediately.
	 */
	UFUNCTION(BlueprintCallable)
	void InterruptCue();

	/**
	 * Advances to the next effect in the sequence.
	 */
	UFUNCTION(BlueprintCallable)
	void NextEffect();

	/**
	 * Goes back to the previous effect in the sequence.
	 */
	UFUNCTION(BlueprintCallable)
	void PreviousEffect();

	/**
	 * Plays the effect at the specified index.
	 * @param Index The index of the effect to play.
	 */
	UFUNCTION(BlueprintCallable)
	void PlayEffectAtIndex(const int32 Index);

protected:
	/**
	 * Finishes the cue sequence and triggers the OnCueFinished event.
	 */
	void FinishCue();
};
