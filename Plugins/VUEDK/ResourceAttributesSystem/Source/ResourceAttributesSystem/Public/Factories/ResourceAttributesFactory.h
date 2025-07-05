// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Attributes/ResourceAttribute.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ResourceAttributesFactory.generated.h"

UCLASS()
class RESOURCEATTRIBUTESSYSTEM_API UResourceAttributesFactory : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/**
	 * Creates a new resource attribute instance using the provided attribute data and manager.
	 * @param AttributeData The data asset containing the configuration for the resource attribute.
	 * @param ResourceAttributesManager The manager that will handle this resource attribute.
	 * @return A new instance of UResourceAttribute initialized with the provided data.
	 */
	UFUNCTION(BlueprintCallable, Category = "Resource Attributes", meta = (DefaultToSelf = "Outer", HidePin = "Outer"))
	static UResourceAttribute* CreateResourceAttribute(UResourceAttributeData* AttributeData, UResourceAttributesManager* ResourceAttributesManager);
};
