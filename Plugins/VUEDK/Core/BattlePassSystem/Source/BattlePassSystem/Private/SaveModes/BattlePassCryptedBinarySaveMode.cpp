// Copyright VUEDK, Inc. All Rights Reserved.

#include "SaveModes/BattlePassCryptedBinarySaveMode.h"
#include "BattlePassSystem.h"
#include "OnlineSubsystem.h"
#include "Interfaces/OnlineIdentityInterface.h"
#include "EncryptionContextOpenSSL.h"
#include "Data/BattlePassesSaveData.h"

void UBattlePassCryptedBinarySaveMode::Save()
{
	const FBattlePassesSaveData SaveData = CreateSaveData();

	TArray<uint8> Bytes;
	if (!SerializeSaveData(SaveData, Bytes))
		return;

	const FString FilePath = SAVE_FILE_FULLPATH + TEXT(".bin");
	FFileHelper::SaveArrayToFile(Bytes, *FilePath);
	const FString BackupFilePath = SAVE_FILE_FULLPATH + TEXT(".bak");
	FFileHelper::SaveArrayToFile(Bytes, *BackupFilePath);
}

void UBattlePassCryptedBinarySaveMode::Load()
{
	const FString FilePath = SAVE_FILE_FULLPATH + TEXT(".bin");
	const FString BackupFilePath = SAVE_FILE_FULLPATH + TEXT(".bak");

	FBattlePassesSaveData SaveData;
	TArray<uint8> EncryptedBytes;
	if (FFileHelper::LoadFileToArray(EncryptedBytes, *FilePath))
	{
		if (DeserializeSaveData(EncryptedBytes, SaveData))
		{
			CompleteLoad(SaveData);
			return;
		}
	}

	SaveData = FBattlePassesSaveData();
	EncryptedBytes.Empty();
	if (FFileHelper::LoadFileToArray(EncryptedBytes, *BackupFilePath))
	{
		if (DeserializeSaveData(EncryptedBytes, SaveData))
		{
			CompleteLoad(SaveData);
			return;
		}

		FailLoad(true);
		return;
	}

	FailLoad(false);
}

bool UBattlePassCryptedBinarySaveMode::SerializeSaveData(FBattlePassesSaveData Data, TArray<uint8>& OutSaveData)
{
	TArray<uint8> Bytes;
	FMemoryWriter MemoryWriter(Bytes, true);
	const UScriptStruct* ScriptStruct = FBattlePassesSaveData::StaticStruct();
	ScriptStruct->SerializeBin(MemoryWriter, &Data);
	if (!EncryptData(Bytes))
		return false;

	OutSaveData = Bytes;
	return true;
}

bool UBattlePassCryptedBinarySaveMode::DeserializeSaveData(TArray<uint8>& Data, FBattlePassesSaveData& OutSaveData)
{
	if (!DecryptData(Data))
		return false;

	FBattlePassesSaveData LoadData;
	FMemoryReader MemoryReader(Data, true);
	const UScriptStruct* ScriptStruct = FBattlePassesSaveData::StaticStruct();
	ScriptStruct->SerializeBin(MemoryReader, &LoadData);
	OutSaveData = LoadData;
	return true;
}

bool UBattlePassCryptedBinarySaveMode::EncryptData(TArray<uint8>& DataToEncrypt)
{
	AddChecksum(DataToEncrypt);
	PadData(DataToEncrypt);

	TArray<uint8> Key;
	if (!GetUserKeySHA256(Key))
	{
		UE_LOG(LogBattlePassSystem, Warning, TEXT("UBattlePassCryptedBinarySaveMode::EncryptData: Failed to get user key for encryption."));
		return false;
	}

	FAES::EncryptData(DataToEncrypt.GetData(), DataToEncrypt.Num(), Key.GetData(), Key.Num());
	return true;
}

bool UBattlePassCryptedBinarySaveMode::DecryptData(TArray<uint8>& DataToDecrypt)
{
	constexpr int32 AESBlockSize = 16; // AES operates on 16-byte blocks, so the data size must be a multiple of this
	if (DataToDecrypt.Num() == 0 || DataToDecrypt.Num() % AESBlockSize != 0) // Ensure data size is valid for AES decryption
	{
		UE_LOG(LogBattlePassSystem, Warning, TEXT("UBattlePassCryptedBinarySaveMode::DecryptData: Invalid data size for decryption."));
		return false;
	}

	TArray<uint8> Key;
	if (!GetUserKeySHA256(Key))
	{
		UE_LOG(LogBattlePassSystem, Warning, TEXT("UBattlePassCryptedBinarySaveMode::DecryptData: Failed to get user key for decryption."));
		return false;
	}

	FAES::DecryptData(DataToDecrypt.GetData(), DataToDecrypt.Num(), Key.GetData(), Key.Num());
	UnpadData(DataToDecrypt);

	if (!VerifyChecksum(DataToDecrypt))
		return false;

	RemoveChecksum(DataToDecrypt);
	return true;
}

void UBattlePassCryptedBinarySaveMode::AddChecksum(TArray<uint8>& Data)
{
	TArray<uint8> Checksum;
	FEncryptionContextOpenSSL CryptoCtx;
	CryptoCtx.CalcSHA256(Data, Checksum);
	Data.Append(Checksum);
}

void UBattlePassCryptedBinarySaveMode::RemoveChecksum(TArray<uint8>& Data)
{
	Data.SetNum(Data.Num() - 32);
}

bool UBattlePassCryptedBinarySaveMode::VerifyChecksum(const TArray<uint8>& EncryptedData)
{
	if (EncryptedData.Num() < 32) // Minimum size to hold checksum
		return false;

	TArray<uint8> ChecksumSaved;
	ChecksumSaved.Append(&EncryptedData[EncryptedData.Num() - 32], 32);

	TArray<uint8> DataToCheck = EncryptedData; // Exclude checksum for verification
	DataToCheck.SetNum(EncryptedData.Num() - 32);

	TArray<uint8> ChecksumCalc; // Recalculate checksum to see if it matches the saved one
	FEncryptionContextOpenSSL CryptoCtx;
	CryptoCtx.CalcSHA256(DataToCheck, ChecksumCalc);

	return ChecksumSaved == ChecksumCalc;
}

void UBattlePassCryptedBinarySaveMode::PadData(TArray<uint8>& DataToPad)
{
	constexpr int32 BlockSize = 16;
	int32 PaddingNeeded = BlockSize - (DataToPad.Num() % BlockSize);
	if (PaddingNeeded == 0)
		PaddingNeeded = BlockSize;

	for (int32 i = 0; i < PaddingNeeded; ++i)
		DataToPad.Add(static_cast<uint8>(PaddingNeeded));
}

void UBattlePassCryptedBinarySaveMode::UnpadData(TArray<uint8>& DataToUnpad)
{
	if (DataToUnpad.Num() == 0) return;

	const uint8 PaddingValue = DataToUnpad.Last();
	if (PaddingValue > 16 || PaddingValue == 0)
		return;

	DataToUnpad.SetNum(DataToUnpad.Num() - PaddingValue);
}

bool UBattlePassCryptedBinarySaveMode::GetUserKeySHA256(TArray<uint8>& OutKey)
{
	FString UserKey = TEXT("UserKey");

#if !UE_EDITOR
	const IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
	if (!Subsystem)
		return false;

	const IOnlineIdentityPtr Identity = Subsystem->GetIdentityInterface();
	if (!Identity.IsValid())
		return false;

	const TSharedPtr<const FUniqueNetId> UserId = Identity->GetUniquePlayerId(0);
	if (!UserId.IsValid())
		return false;

	UserKey = UserId->ToString();
#endif

	const FTCHARToUTF8 Converter((UserKey));
	TArray<uint8> UserIdBytes;
	UserIdBytes.Append(reinterpret_cast<const uint8*>(Converter.Get()), Converter.Length());

	TArray<uint8> Key;
	FEncryptionContextOpenSSL CryptoCtx;
	CryptoCtx.CalcSHA256(UserIdBytes, Key);
	OutKey = Key;
	return true;
}
