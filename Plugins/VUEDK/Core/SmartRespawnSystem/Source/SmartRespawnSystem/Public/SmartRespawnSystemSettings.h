// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Objects/SmartRespawnBehaviours/Base/SmartRespawnBehaviour.h"
#include "Engine/DeveloperSettings.h"
#include "Misc/App.h"
#include "Objects/RegisterSavers/SmartRespawnRegisterSlotSaver.h"
#include "Objects/RegisterSavers/Base/SmartRespawnRegisterSaverBase.h"
#include "Objects/RespawnSelectors/SmartRespawnSelectorMostRecent.h"
#include "Objects/RespawnSelectors/Base/SmartRespawnSelectorBase.h"
#include "Objects/SmartRespawnBehaviours/SmartRespawnTranslationBehaviour.h"
#include "SmartRespawnSystemSettings.generated.h"

UENUM(BlueprintType, meta = (Bitflags))
enum class EPostLoginRespawnContext : uint8
{
	None = 0 UMETA(Hidden),
	Shipping = 1 << 0 UMETA(DisplayName = "Shipping"),
	Development = 1 << 1 UMETA(DisplayName = "Development"),
	DebugGame = 1 << 2 UMETA(DisplayName = "DebugGame"),
	Editor = 1 << 3 UMETA(DisplayName = "Editor")
};

ENUM_CLASS_FLAGS(EPostLoginRespawnContext)

UCLASS(Config = SmartRespawnSettings, DefaultConfig, meta = (DisplayName = "Smart Respawn System"))
class SMARTRESPAWNSYSTEM_API USmartRespawnSystemSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	UPROPERTY(Config, EditAnywhere, Category = "Respawn")
	TSoftClassPtr<USmartRespawnSelectorBase> RespawnSelectorClass = USmartRespawnSelectorMostRecent::StaticClass();
	UPROPERTY(Config, EditAnywhere, Category = "Respawn")
	TSoftClassPtr<USmartRespawnBehaviour> RespawnBehaviourClass = USmartRespawnTranslationBehaviour::StaticClass();
	UPROPERTY(Config, EditAnywhere, Category = "Respawn",
		meta = (ToolTip = "If enabled, the system will disable player input when respawn requested and enable it again on respawn start."))
	bool bRespawnDisableInput = true;
	UPROPERTY(Config, EditAnywhere, Category = "Respawn", meta = (
			ToolTip = "If the current context is not included in the bitmask, the system will not try to respawn the player on post login.",
			Bitmask,
			BitmaskEnum = "/Script/SmartRespawnSystem.EPostLoginRespawnContext")
	)
	uint32 PostLoginRespawnContexts =
		1 << static_cast<uint8>(EPostLoginRespawnContext::Shipping) |
		1 << static_cast<uint8>(EPostLoginRespawnContext::Development) |
		1 << static_cast<uint8>(EPostLoginRespawnContext::DebugGame) |
		1 << static_cast<uint8>(EPostLoginRespawnContext::Editor);
	UPROPERTY(Config, EditAnywhere, Category = "Register")
	TSoftClassPtr<USmartRespawnRegisterSaverBase> RegisterSaverClass = USmartRespawnRegisterSlotSaver::StaticClass();
	UPROPERTY(Config, EditAnywhere, Category = "Register",
		meta = (ToolTip = "If enabled, the system will automatically save the respawn register when a new respawn is registered or unregistered."))
	bool bNewEntrySave = true;
	UPROPERTY(Config, EditAnywhere, Category = "Register",
		meta = (ToolTip = "If enabled, the system will try to restore the respawn register on post login if it was not already restored."))
	bool bPostLoginRestore = true;

public:
	USmartRespawnSystemSettings();
};
