// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "CustomOptionObject.generated.h"

UCLASS(Abstract, Blueprintable, NotBlueprintType)
class CUSTOMSETTINGSSYSTEM_API UCustomOptionObject : public UObject
{
	GENERATED_BODY()

	friend class UCustomSettingsSubsystem;
	
private:
	UPROPERTY()
	UWorld* CachedWorld;
	
public:
	void Init(UWorld* InWorld);

protected:
	UFUNCTION(BlueprintNativeEvent)
	void OnInit();
	
	UFUNCTION(BlueprintNativeEvent)
	void OnCustomSettingsApplied(const float Value);

	UFUNCTION(BlueprintNativeEvent)
	void OnCustomOptionChanged(const float Value);

#if WITH_EDITOR
	virtual bool ImplementsGetWorld() const override;
#endif

	virtual UWorld* GetWorld() const override;
};
