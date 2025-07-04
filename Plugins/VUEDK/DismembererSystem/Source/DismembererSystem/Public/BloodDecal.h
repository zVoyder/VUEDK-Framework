// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BloodDecal.generated.h"

class UBloodDecalsPool;

UCLASS(NotBlueprintable, BlueprintType)
class DISMEMBERERSYSTEM_API ABloodDecal : public AActor
{
	GENERATED_BODY()

protected:
	UPROPERTY()
	UMaterialInstanceDynamic* DynamicDecalMaterial;
	
private:
	UPROPERTY()
	UDecalComponent* DecalComponent;
	UPROPERTY()
	UBloodDecalsPool* BloodDecalsPool;
	bool bIsSpawned = false;
	float Duration = 0.f;
	float CurrentTimer = 0.f;
	float FadeInTime = 0.f;
	float FadeOutTime = 0.f;
	FName DecalFadeParameterName = TEXT("DecalFade");

public:
	ABloodDecal();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

	void Init(UMaterialInterface* DecalMaterial, const FVector& NormalOrientation, const FVector& Size, float DecalDuration, int SortOrder, float FadeIn, float FadeOut);

protected:
	void OnDecalLifeTimeProcess(float Timer, float InDuration) const;
	
	void OnDecalFadeIn(const float Timer, const float InFadeInTime, const float InDuration) const;
	
	void OnDecalFadeOut(const float Timer, const float InFadeOutTime, const float InDuration) const;
	
private:
	void StartDecalLifeTime(const float DecalDuration, const float InFadeInTime, const float InFadeOutTime);

	void ProcessDecalLifeTime(float DeltaSeconds);

	void EndDecalLifeTime();

	bool Check() const;
};
