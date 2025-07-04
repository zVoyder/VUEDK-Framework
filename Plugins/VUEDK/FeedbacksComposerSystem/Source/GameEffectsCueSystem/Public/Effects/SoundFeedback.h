// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameEffectBase.h"
#include "SoundFeedback.generated.h"

UCLASS()
class GAMEEFFECTSCUESYSTEM_API USoundFeedback : public UGameEffectBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameEffect|Sound")
	bool bIs2D = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameEffect|Sound")
	bool bIsUISound = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameEffect|Sound")
	USoundBase* Sound = nullptr;
	float VolumeMultiplier = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameEffect|Sound")
	float PitchMultiplier = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameEffect|Sound")
	float StartTime = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameEffect|Sound")
	USoundAttenuation* AttenuationSettings = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameEffect|Sound")
	USoundConcurrency* ConcurrencySettings = nullptr;

protected:
	virtual void OnPlay_Implementation() override;
};
