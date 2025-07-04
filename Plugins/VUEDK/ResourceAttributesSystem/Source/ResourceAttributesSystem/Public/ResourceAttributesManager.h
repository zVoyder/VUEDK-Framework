// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Attributes/ResourceAttribute.h"
#include "Components/ActorComponent.h"
#include "ResourceAttributesManager.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class RESOURCEATTRIBUTESSYSTEM_API UResourceAttributesManager : public UActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource Attributes")
	TSet<UResourceAttributeData*> StartingAttributes;

private:
	UPROPERTY()
	TArray<UResourceAttribute*> CurrentAttributes;
	
public:
	UResourceAttributesManager();

	UFUNCTION(BlueprintCallable)
	bool TryAddAttribute(UResourceAttributeData* AttributeData, UResourceAttribute*& OutAttribute);

	UFUNCTION(BlueprintCallable)
	bool TryRemoveAttributeByTag(const FGameplayTag Tag);

	UFUNCTION(BlueprintPure)
	UResourceAttribute* GetAttributeByTag(const FGameplayTag Tag) const;

	UFUNCTION(BlueprintCallable)
	TArray<UResourceAttribute*> GetAttributes() const;

	UFUNCTION(BlueprintCallable, BlueprintPure = false)
	bool TrySetAttributeValueByTag(const FGameplayTag Tag, const float NewValue) const;

	UFUNCTION(BlueprintCallable, BlueprintPure = false)
	bool TryModifyAttributeValueByTag(const FGameplayTag Tag, const float Amount) const;

protected:
	virtual void BeginPlay() override;

private:
	void Init();
};
