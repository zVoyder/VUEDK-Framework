// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "GameEffectBase.generated.h"

class UGameEffectsCue;

UCLASS(Abstract, Blueprintable, BlueprintType, EditInlineNew)
class GAMEEFFECTSCUESYSTEM_API UGameEffectBase : public UObject, public FTickableGameObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0.0", UIMin = "0.0"), Category = "GameEffect|Delay")
	float InDelay = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0.0", UIMin = "0.0"), Category = "GameEffect|Delay")
	float OutDelay = 0.0f;

private:
	UPROPERTY()
	TObjectPtr<AActor> Instigator;
	UPROPERTY()
	UObject* Payload;
	UPROPERTY()
	UGameEffectsCue* RelatedCue;
	bool bIsPlaying = false;
	FTimerHandle DelayTimerHandle;

public:
	void Init(AActor* InInstigator, UObject* InPayload, UGameEffectsCue* InRelatedCue);

	/**
	 * @brief Starts the effect after the InDelay time.
	 */
	UFUNCTION(BlueprintCallable)
	void Play();

	/**
	 * @brief Stops the effect after the OutDelay time.
	 */
	UFUNCTION(BlueprintCallable)
	void Stop();

	UFUNCTION(BlueprintCallable)
	void Pause();

	void CueStart();
	
	void CueStop();

	void CueResume();
	
	void CuePause();
	
	virtual void Tick(float DeltaTime) override;

	virtual bool IsTickable() const override;

	virtual bool IsTickableInEditor() const override;
	
	virtual TStatId GetStatId() const override;

protected:
#if WITH_EDITOR
	virtual bool ImplementsGetWorld() const override;
#endif

	virtual UWorld* GetWorld() const override;

	/**
	 * Finishes the effect and flags it as finished to pass to the next effect.
	 */
	UFUNCTION(BlueprintCallable)
	void FinishExecute();

	/**
	 * Advances to the next effect in the sequence.
	 */
	UFUNCTION(BlueprintNativeEvent)
	void NextEffect();

	/**
	 * Returns the instigator actor for this effect.
	 * @return Pointer to the instigator AActor.
	 */
	UFUNCTION(BlueprintPure)
	AActor* GetInstigator() const;

	/**
	 * Returns the payload object for this effect.
	 * @return Pointer to the payload UObject.
	 */
	UFUNCTION(BlueprintPure)
	UObject* GetPayload() const;

	/**
	 * Returns the related cue for this effect.
	 * @return Pointer to the related UGameEffectsCue.
	 */
	UFUNCTION(BlueprintPure)
	UGameEffectsCue* GetRelatedCue() const;

	/**
	 * Returns the out delay for this effect.
	 * @return Out delay in seconds.
	 */
	UFUNCTION(BlueprintPure)
	float GetOutDelay() const;

	/**
	 * Returns the in delay for this effect.
	 * @return In delay in seconds.
	 */
	UFUNCTION(BlueprintPure)
	float GetInDelay() const;

	/**
	 * Called when the effect is initialized.
	 */
	UFUNCTION(BlueprintNativeEvent)
	void OnInit();

	/**
	 * Called when the effect is played.
	 */
	UFUNCTION(BlueprintNativeEvent)
	void OnPlay();

	/**
	 * Called when the effect is stopped.
	 */
	UFUNCTION(BlueprintNativeEvent)
	void OnStop();

	UFUNCTION(BlueprintNativeEvent)
	void OnPause();

	UFUNCTION(BlueprintNativeEvent)
	void OnCueStart();
	
	UFUNCTION(BlueprintNativeEvent)
	void OnCueStop();

	UFUNCTION(BlueprintNativeEvent)
	void OnCuePause();

	UFUNCTION(BlueprintNativeEvent)
	void OnCueResume();

	/**
	 * Called every tick while the effect is active.
	 * @param DeltaTime Time elapsed since last tick.
	 */
	UFUNCTION(BlueprintNativeEvent)
	void OnTick(float DeltaTime);

	/**
	 * Called when the effect finishes.
	 */
	UFUNCTION(BlueprintNativeEvent)
	void OnFinish();

private:
	void SetPlay();
	
	void SetFinish();
	
	void SetStop();

	void SetPause();

	void ClearTimer();

	bool Check() const;
};
