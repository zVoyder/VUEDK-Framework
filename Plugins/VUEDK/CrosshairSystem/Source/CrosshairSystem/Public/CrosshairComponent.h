// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Widgets/CrosshairWidget.h"
#include "CrosshairComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class CROSSHAIRSYSTEM_API UCrosshairComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Crosshair")
	TSubclassOf<UCrosshairWidget> CrosshairWidgetClass;

protected:
	UPROPERTY()
	UObject* CrosshairPayload;

public:
	UCrosshairComponent();

	UFUNCTION(BlueprintCallable)
	void SetPayload(UObject* Payload);

	UFUNCTION(BlueprintCallable)
	void SetDefaultCrosshair() const;
	
	UFUNCTION(BlueprintCallable)
	void SetCrosshair() const;

	UFUNCTION(BlueprintCallable)
	void ShowCrosshair() const;

	UFUNCTION(BlueprintCallable)
	void HideCrosshair() const;

	UFUNCTION(BlueprintCallable)
	void CloseCrosshairInViewport() const;
};
