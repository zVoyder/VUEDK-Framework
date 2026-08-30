// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AssetTypeActions_Base.h"

class VGOAPEDITOR_API FAssetTypeActions_GOAPBase : public FAssetTypeActions_Base
{
	
public:
	virtual FText GetName() const override;
	
	virtual FColor GetTypeColor() const override;
	
	virtual uint32 GetCategories() override;
};
