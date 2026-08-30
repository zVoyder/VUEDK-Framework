// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Base/AssetTypeActions_GOAPBase.h"

class VGOAPEDITOR_API FAssetTypeActions_Behaviour : public FAssetTypeActions_GOAPBase
{
public:
	virtual UClass* GetSupportedClass() const override;

	virtual FColor GetTypeColor() const override;

	virtual FText GetName() const override;

	virtual const FSlateBrush* GetThumbnailBrush(const FAssetData& InAssetData, const FName InClassName) const override;
};
