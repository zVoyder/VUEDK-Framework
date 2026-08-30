// Copyright VUEDK, Inc. All Rights Reserved.

#include "Factories/ResourceAttributesFactory.h"
#include "ResourceAttributesManager.h"

UResourceAttribute* UResourceAttributesFactory::CreateResourceAttribute(UResourceAttributeData* AttributeData, UResourceAttributesManager* ResourceAttributesManager)
{
	if (!IsValid(AttributeData) || !IsValid(ResourceAttributesManager))
		return nullptr;

	UResourceAttribute* NewAttribute = NewObject<UResourceAttribute>(ResourceAttributesManager);
	if (NewAttribute)
		NewAttribute->Init(AttributeData, ResourceAttributesManager);
	
	return NewAttribute;
}
