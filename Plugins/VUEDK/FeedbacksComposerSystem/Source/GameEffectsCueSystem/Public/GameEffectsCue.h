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
	void Init(AActor* Instigator);

	UFUNCTION(BlueprintCallable)
	void StartCue();

	UFUNCTION(BlueprintCallable)
	void StopCue();

	UFUNCTION(BlueprintCallable)
	void ResumeCue();

	UFUNCTION(BlueprintCallable)
	void InterruptCue();

	UFUNCTION(BlueprintCallable)
	void NextEffect();

	UFUNCTION(BlueprintCallable)
	void PreviousEffect();

	UFUNCTION(BlueprintCallable)
	void PlayEffectAtIndex(const int32 Index);

protected:
	void FinishCue();
};
