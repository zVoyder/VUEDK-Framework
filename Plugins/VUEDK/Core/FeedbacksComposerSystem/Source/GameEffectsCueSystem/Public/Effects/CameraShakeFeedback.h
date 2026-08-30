// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Effects/Base/GameEffectBase.h"
#include "CameraShakeFeedback.generated.h"

UCLASS()
class GAMEEFFECTSCUESYSTEM_API UCameraShakeFeedback : public UGameEffectBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameEffect|Camera Shake")
	TSubclassOf<UCameraShakeBase> CameraShakeClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameEffect|Camera Shake")
	float Scale = 1.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameEffect|Camera Shake")
	ECameraShakePlaySpace PlaySpace = ECameraShakePlaySpace::CameraLocal;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameEffect|Camera Shake")
	FRotator UserPlaySpaceRot;

protected:
	virtual void OnPlay_Implementation() override;
};
