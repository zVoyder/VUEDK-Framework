// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Base/BattlePassSaveModeBase.h"
#include "BattlePassLocalJsonSaveMode.generated.h"

/**
 * @brief JSON-based local save mode for battle pass progression.
 * Saves and loads battle pass data to a local file in JSON format.
 * Unsafe for production use due to potential security risks and lack of encryption, but useful for testing and debugging.
 */
UCLASS(NotBlueprintable)
class BATTLEPASSSYSTEM_API UBattlePassLocalJsonSaveMode : public UBattlePassSaveModeBase
{
	GENERATED_BODY()

public:
	virtual void Save() override;

	virtual void Load() override;
};
