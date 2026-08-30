// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "DismemberLimbOperation.h"
#include "Effects/BloodDecal.h"
#include "UObject/Object.h"
#include "DismembererSystemSettings.generated.h"

UCLASS(Config = DismembererSystemSettings, DefaultConfig, meta = (DisplayName = "Dismemberer System Settings"))
class DISMEMBERERSYSTEM_API UDismembererSystemSettings : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(Config, EditAnywhere, Category = "Settings|Limbs")
	TSoftClassPtr<ADismemberedLimb> DismemberedLimb = ADismemberedLimb::StaticClass();
	UPROPERTY(Config, EditAnywhere, Category = "Settings|Limbs")
	TSoftClassPtr<UDismemberLimbOperation> DismemberLimbOperation = UDismemberLimbOperation::StaticClass();
	UPROPERTY(Config, EditAnywhere, Category = "Settings|Limbs")
	float DismemberedLimbLifeSpan = 60.0f;
	UPROPERTY(Config, EditAnywhere, Category = "Settings|Limbs")
	FCollisionProfileName LimbCollisionProfile = FCollisionProfileName("Ragdoll");
	UPROPERTY(Config, EditAnywhere, Category = "Settings|Decals")
	TSoftClassPtr<ABloodDecal> BloodDecal = ABloodDecal::StaticClass();
	UPROPERTY(Config, EditAnywhere, Category = "Settings|Decals")
	TArray<TSoftClassPtr<AGameModeBase>> AllowedDecalGamemodes = { TSoftClassPtr<AGameModeBase>(FSoftClassPath("/Script/Engine.GameModeBase")) };
	UPROPERTY(Config, EditAnywhere, Category = "Settings|Decals")
	int32 DecalPoolSize = 100;
	UPROPERTY(Config, EditAnywhere, meta = (ClampMin = 0.f, UIMin = 0.f), Category = "Settings|Decals")
	float DecalSpawnRate = .05f;
	UPROPERTY(Config, EditAnywhere, Category = "Settings|Decals")
	float DecalsDuration = 30.0f;
	UPROPERTY(Config, EditAnywhere, Category = "Settings|Decals")
	float DecalFadeInTime = 0.1f;
	UPROPERTY(Config, EditAnywhere, Category = "Settings|Decals")
	float DecalFadeOutTime = 1.0f;
	UPROPERTY(Config, EditAnywhere, Category = "Settings|Decals")
	FName DecalFadeParameterName = TEXT("DecalFade");
};
