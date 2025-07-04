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
	UFUNCTION(BlueprintCallable, Category = "Resource Attributes", meta = (DefaultToSelf = "Outer", HidePin = "Outer"))
	static UResourceAttribute* CreateResourceAttribute(UResourceAttributeData* AttributeData, UResourceAttributesManager* ResourceAttributesManager);
};
