// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Base/BattlePassSaveModeBase.h"
#include "BattlePassCryptedBinarySaveMode.generated.h"

/**
 * @brief AES-encrypted binary save mode for battle pass progression.
 * This save mode uses AES-256 encryption to protect the saved data from unauthorized access.
 * The save data is serialized into a binary format before encryption, and deserialized after decryption during loading.
 */
UCLASS()
class BATTLEPASSSYSTEM_API UBattlePassCryptedBinarySaveMode : public UBattlePassSaveModeBase
{
	GENERATED_BODY()

public:
	virtual void Save() override;

	virtual void Load() override;

private:
	static bool SerializeSaveData(FBattlePassesSaveData Data, TArray<uint8>& OutSaveData);

	static bool DeserializeSaveData(TArray<uint8>& Data, FBattlePassesSaveData& OutSaveData);

	static bool EncryptData(TArray<uint8>& DataToEncrypt);

	static bool DecryptData(TArray<uint8>& DataToDecrypt);

	static void AddChecksum(TArray<uint8>& Data);

	static void RemoveChecksum(TArray<uint8>& Data);

	static bool VerifyChecksum(const TArray<uint8>& EncryptedData);
	
	static void PadData(TArray<uint8>& DataToPad);

	static void UnpadData(TArray<uint8>& DataToUnpad);
	
	static bool GetUserKeySHA256(TArray<uint8>& OutKey);
};
