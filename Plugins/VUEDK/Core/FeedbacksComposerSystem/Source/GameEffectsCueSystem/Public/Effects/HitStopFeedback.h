// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Base/GameEffectBase.h"
#include "HitStopFeedback.generated.h"

UCLASS()
class GAMEEFFECTSCUESYSTEM_API UHitStopFeedback : public UGameEffectBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameEffect|Hit Stop")
	float TimeDilation;

private:
	float OriginalTimeDilation;
	
protected:
	virtual void OnPlay_Implementation() override;
	
	virtual void OnStop_Implementation() override;
	
	virtual void OnFinish_Implementation() override;
};
