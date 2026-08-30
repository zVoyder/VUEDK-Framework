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

    /**
    * Attempts to add a new attribute to the manager using the provided data asset.
    * @param AttributeData The data asset for the attribute to add.
    * @param OutAttribute Reference to the newly created attribute if successful.
    * @return True if the attribute was added, false otherwise.
    */
	UFUNCTION(BlueprintCallable)
	bool TryAddAttribute(UResourceAttributeData* AttributeData, UResourceAttribute*& OutAttribute);

    /**
    * Attempts to remove an attribute from the manager by its gameplay tag.
    * @param Tag The gameplay tag identifying the attribute to remove.
    * @return True if the attribute was removed, false otherwise.
    */
	UFUNCTION(BlueprintCallable)
	bool TryRemoveAttributeByTag(const FGameplayTag Tag);

    /**
    * Retrieves an attribute by its gameplay tag.
    * @param Tag The gameplay tag to search for.
    * @return Pointer to the attribute if found, nullptr otherwise.
    */
	UFUNCTION(BlueprintPure)
	UResourceAttribute* GetAttributeByTag(const FGameplayTag Tag) const;

    /**
    * Returns an array of all current attributes managed by this component.
    */
	UFUNCTION(BlueprintCallable)
	TArray<UResourceAttribute*> GetAttributes() const;

    /**
    * Attempts to set the value of an attribute identified by its gameplay tag.
    * @param Tag The gameplay tag of the attribute.
    * @param NewValue The new value to set.
    * @return True if the value was set, false otherwise.
    */
	UFUNCTION(BlueprintCallable, BlueprintPure = false)
	bool TrySetAttributeValueByTag(const FGameplayTag Tag, const float NewValue) const;

    /**
    * Attempts to modify the value of an attribute identified by its gameplay tag.
    * @param Tag The gameplay tag of the attribute.
    * @param Amount The amount to modify the value by.
    * @return True if the value was modified, false otherwise.
    */
	UFUNCTION(BlueprintCallable, BlueprintPure = false)
	bool TryModifyAttributeValueByTag(const FGameplayTag Tag, const float Amount) const;

protected:
	virtual void BeginPlay() override;

private:
    /**
    * Initializes the manager and its attributes. Called internally.
    */
	void Init();
};
