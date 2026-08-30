// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Base/AssetTypeActions_GOAPBase.h"

class VGOAPEDITOR_API FAssetTypeActions_Action : public FAssetTypeActions_GOAPBase
{
public:
	virtual UClass* GetSupportedClass() const override;

	virtual FText GetName() const override;

	virtual const TArray<FText>& GetSubMenus() const override;
};
