// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BloodDecal.generated.h"

class UBloodDecalsPool;

UCLASS(Blueprintable, BlueprintType)
class DISMEMBERERSYSTEM_API ABloodDecal : public AActor
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly)
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

	virtual void Tick(float DeltaSeconds) override;

	/**
	 * Initializes the blood decal with the specified parameters.
	 * @param DecalMaterial The material to use for the decal.
	 * @param NormalOrientation The normal orientation of the decal.
	 * @param Size The size of the decal.
	 * @param DecalDuration The duration the decal will be visible.
	 * @param SortOrder The sort order for rendering the decal.
	 * @param FadeIn Time for the decal to fade in.
	 * @param FadeOut Time for the decal to fade out.
	 */
	void Init(UMaterialInterface* DecalMaterial, const FVector& NormalOrientation, const FVector& Size, float DecalDuration, int SortOrder, float FadeIn, float FadeOut);

protected:
	virtual void BeginPlay() override;
	
	/**
	 * Handles the decal's lifetime process.
	 * @param Timer Current timer value.
	 * @param InDuration Total duration of the decal.
	 */
	void OnDecalLifeTimeProcessNative(float Timer, float InDuration) const;

	/**
	 * Handles the fade-in effect of the decal.
	 * @param Timer Current timer value.
	 * @param InFadeInTime Fade-in duration.
	 * @param InDuration Total duration of the decal.
	 */
	void OnDecalFadeInNative(const float Timer, const float InFadeInTime, const float InDuration) const;

	/**
	 * Handles the fade-out effect of the decal.
	 * @param Timer Current timer value.
	 * @param InFadeOutTime Fade-out duration.
	 * @param InDuration Total duration of the decal.
	 */
	void OnDecalFadeOutNative(const float Timer, const float InFadeOutTime, const float InDuration) const;

	UFUNCTION(BlueprintNativeEvent)
	void OnInit();
	
	/**
	 * Handles the decal's lifetime process.
	 * @param Timer Current timer value.
	 * @param InDuration Total duration of the decal.
	 */
	UFUNCTION(BlueprintNativeEvent)
	void OnDecalLifeTimeProcess(float Timer, float InDuration) const;

	/**
	 * Handles the fade-in effect of the decal.
	 * @param Timer Current timer value.
	 * @param InFadeInTime Fade-in duration.
	 * @param InDuration Total duration of the decal.
	 */
	UFUNCTION(BlueprintNativeEvent)
	void OnDecalFadeIn(const float Timer, const float InFadeInTime, const float InDuration) const;

	/**
	 * Handles the fade-out effect of the decal.
	 * @param Timer Current timer value.
	 * @param InFadeOutTime Fade-out duration.
	 * @param InDuration Total duration of the decal.
	 */
	UFUNCTION(BlueprintNativeEvent)
	void OnDecalFadeOut(const float Timer, const float InFadeOutTime, const float InDuration) const;

private:
	/**
	 * Starts the decal's lifetime process.
	 * @param DecalDuration Total duration of the decal.
	 * @param InFadeInTime Fade-in duration.
	 * @param InFadeOutTime Fade-out duration.
	 */
	void StartDecalLifeTime(const float DecalDuration, const float InFadeInTime, const float InFadeOutTime);

	/**
	 * Processes the decal's lifetime each frame.
	 * @param DeltaSeconds Time elapsed since last frame.
	 */
	void ProcessDecalLifeTime(float DeltaSeconds);

	/**
	 * Ends the decal's lifetime and handles cleanup.
	 */
	void EndDecalLifeTime();

	/**
	 * Checks the validity or state of the decal.
	 * @return True if valid, false otherwise.
	 */
	bool Check() const;
};
