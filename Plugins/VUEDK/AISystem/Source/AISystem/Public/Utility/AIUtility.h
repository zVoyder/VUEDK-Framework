// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Perception/AIPerceptionComponent.h"
#include "AIUtility.generated.h"

UCLASS()
class AISYSTEM_API UAIUtility : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "VUEDK|AI Utility")
	static void SetPeripheralVisionAngle (UAIPerceptionComponent* PerceptionComponent, float PeripheralVisionAngle);

	UFUNCTION(BlueprintCallable, Category = "VUEDK|AI Utility")
	static UAISenseConfig* GetSenseConfig(UAIPerceptionComponent* PerceptionComponent, TSubclassOf<UAISense> SenseClass);

	UFUNCTION(BlueprintCallable, Category = "VUEDK|AI Utility")
	static void SetSightRadius(UAIPerceptionComponent* PerceptionComponent, float SightRadius);

	UFUNCTION(BlueprintCallable, Category = "VUEDK|AI Utility")
	static void SetLoseSightRadius(UAIPerceptionComponent* PerceptionComponent, float LoseSightRadius);
	
	UFUNCTION(BlueprintCallable, Category = "VUEDK|AI Utility")
	static void RequestStimuliListenerUpdate(UAIPerceptionComponent* PerceptionComponent);
};
