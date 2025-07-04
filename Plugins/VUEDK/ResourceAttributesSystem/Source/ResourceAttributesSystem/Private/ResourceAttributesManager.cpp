// Copyright VUEDK, Inc. All Rights Reserved.

#include "ResourceAttributesManager.h"
#include "Factories/ResourceAttributesFactory.h"

UResourceAttributesManager::UResourceAttributesManager()
{
	PrimaryComponentTick.bCanEverTick = false;
}

bool UResourceAttributesManager::TryAddAttribute(UResourceAttributeData* AttributeData, UResourceAttribute*& OutAttribute)
{
	if (!IsValid(AttributeData))
	{
		OutAttribute = nullptr;
		return false;
	}

	UResourceAttribute* NewAttribute = UResourceAttributesFactory::CreateResourceAttribute(AttributeData, this);
	if (!IsValid(NewAttribute))
	{
		OutAttribute = nullptr;
		return false;
	}

	CurrentAttributes.Add(NewAttribute);
	OutAttribute = NewAttribute;
	return true;
}

bool UResourceAttributesManager::TryRemoveAttributeByTag(const FGameplayTag Tag)
{
	UResourceAttribute* Attribute = GetAttributeByTag(Tag);

	if (!IsValid(Attribute))
		return false;

	CurrentAttributes.Remove(Attribute);
	return true;
}

UResourceAttribute* UResourceAttributesManager::GetAttributeByTag(const FGameplayTag Tag) const
{
	for (UResourceAttribute* Attribute : CurrentAttributes)
	{
		if (!IsValid(Attribute) || !IsValid(Attribute->ResourceAttributeData))
			continue;

		if (Attribute->ResourceAttributeData->AttributeTag == Tag)
			return Attribute;
	}
	
	return nullptr;
}

TArray<UResourceAttribute*> UResourceAttributesManager::GetAttributes() const
{
	return CurrentAttributes;
}

bool UResourceAttributesManager::TrySetAttributeValueByTag(const FGameplayTag Tag, const float NewValue) const
{
	UResourceAttribute* Attribute = GetAttributeByTag(Tag);

	if (!IsValid(Attribute))
		return false;

	Attribute->SetValue(NewValue);
	return true;
}

bool UResourceAttributesManager::TryModifyAttributeValueByTag(const FGameplayTag Tag, const float DeltaValue) const
{
	UResourceAttribute* Attribute = GetAttributeByTag(Tag);

	if (!IsValid(Attribute))
		return false;

	Attribute->ModifyValue(DeltaValue);
	return true;
}

void UResourceAttributesManager::BeginPlay()
{
	Super::BeginPlay();
	Init();
}

void UResourceAttributesManager::Init()
{
	for (UResourceAttributeData* AttributeData : StartingAttributes)
	{
		if (!IsValid(AttributeData))
			continue;

		UResourceAttribute* NewAttribute = UResourceAttributesFactory::CreateResourceAttribute(AttributeData, this);
		CurrentAttributes.Add(NewAttribute);
	}
}
