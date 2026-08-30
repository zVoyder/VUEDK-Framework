// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Effects/Base/GameEffectBase.h"
#include "UObject/Object.h"
#include "GameEffectsCue.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(
	FOnCueStarted
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(
	FOnCueStopped
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(
	FOnCuePaused
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(
	FOnCueResumed
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(
	FOnCueFinished
);

DECLARE_DELEGATE_OneParam(
	FOnCueCompleted,
	UGameEffectsCue* /* GameEffectCue */
);

UCLASS(NotBlueprintable, BlueprintType, EditInlineNew, DefaultToInstanced)
class GAMEEFFECTSCUESYSTEM_API UGameEffectsCue : public UObject
{
	GENERATED_BODY()

	friend class UGameEffectsSubsystem;

public:
	UPROPERTY(BlueprintAssignable, Category = Events)
	FOnCueStarted OnCueStarted;
	UPROPERTY(BlueprintAssignable, Category = Events)
	FOnCueStopped OnCueStopped;
	UPROPERTY(BlueprintAssignable, Category = Events)
	FOnCuePaused OnCuePaused;
	UPROPERTY(BlueprintAssignable, Category = Events)
	FOnCueResumed OnCueResumed;
	UPROPERTY(BlueprintAssignable, Category = Events)
	FOnCueFinished OnCueFinished;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GameEffects")
	int32 EffectStartIndex = 0;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Instanced, Category = "GameEffects")
	TArray<UGameEffectBase*> Effects;

private:
	FOnCueCompleted OnCueCompleted;
	UPROPERTY()
	AActor* Instigator;
	bool bIsPlaying = false;
	int32 CurrentIndex = 0;

public:
	/**
	 * Initializes the cue with the given instigator.
	 * @param InInstigator The actor that instigates the cue.
	 * @param Payload Optional payload object that can be used by the effects in the cue.
	 */
	void Init(AActor* InInstigator, UObject* Payload = nullptr);

	/**
	 * @brief Starts the cue sequence.
	 * @param bSkipIfPlaying If true, the cue will not start if it is already playing.
	 */
	UFUNCTION(BlueprintCallable, meta = (AdvancedDisplay = "bSkipIfPlaying"))
	void StartCue(const bool bSkipIfPlaying = false);

	/**
	 * @brief Starts the cue sequence from a specific effect index.
	 * @param Index The index of the effect to start from.
	 * @param bSkipIfPlaying If true, the cue will not start if it is already playing.
	 */
	UFUNCTION(BlueprintCallable)
	void StartCueAtIndex(const int32 Index, const bool bSkipIfPlaying = false);

	/**
	 * Stops the cue sequence.
	 */
	UFUNCTION(BlueprintCallable)
	void StopCue();

	UFUNCTION(BlueprintCallable)
	void PauseCue();

	/**
	 * Resumes the cue sequence from the current effect.
	 */
	UFUNCTION(BlueprintCallable)
	void ResumeCue();
	
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
	 * Checks if the cue is currently playing.
	 * @return True if the cue is playing, false otherwise.
	 */
	UFUNCTION(BlueprintPure)
	bool IsPlaying() const;

	virtual class UWorld* GetWorld() const override;

protected:
	void FinishCue();

private:
	void PlayEffectAtIndex(const int32 Index);
};
