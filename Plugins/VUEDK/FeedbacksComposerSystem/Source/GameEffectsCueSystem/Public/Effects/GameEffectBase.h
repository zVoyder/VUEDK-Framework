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
	AActor* Instigator;
	UPROPERTY()
	UGameEffectsCue* RelatedCue;
	bool bIsPlaying = false;
	FTimerHandle DelayTimerHandle;

public:
	void Init(AActor* InInstigator, UGameEffectsCue* InRelatedCue);
	
	UFUNCTION(BlueprintCallable)
	void Play();

	/**
	 * @brief Stops the effect after the OutDelay time.
	 */
	UFUNCTION(BlueprintCallable)
	void Stop();

	/**
	 * @brief Interrupts the effect immediately.
	 */
	UFUNCTION(BlueprintCallable)
	void Interrupt();

	virtual void Tick(float DeltaTime) override;

	virtual bool IsTickable() const override;

	virtual TStatId GetStatId() const override;

protected:
#if WITH_EDITOR
	virtual bool ImplementsGetWorld() const override;
#endif

	virtual UWorld* GetWorld() const override;
	
	UFUNCTION(BlueprintCallable)
	void Finish();

	UFUNCTION(BlueprintNativeEvent)
	void NextEffect();
	
	UFUNCTION(BlueprintPure)
	AActor* GetInstigator() const;

	UFUNCTION(BlueprintPure)
	UGameEffectsCue* GetRelatedCue() const;

	UFUNCTION(BlueprintPure)
	float GetOutDelay() const;

	UFUNCTION(BlueprintPure)
	float GetInDelay() const;
	
	UFUNCTION(BlueprintNativeEvent)
	void OnInit();

	UFUNCTION(BlueprintNativeEvent)
	void OnPlay();

	UFUNCTION(BlueprintNativeEvent)
	void OnStop();
	
	UFUNCTION(BlueprintNativeEvent)
	void OnTick(float DeltaTime);
	
	UFUNCTION(BlueprintNativeEvent)
	void OnFinish();
	
private:
	void SetPlay();

	void SetFinish();

	void SetStop();

	void ClearTimer();
	
	bool Check() const;
};
