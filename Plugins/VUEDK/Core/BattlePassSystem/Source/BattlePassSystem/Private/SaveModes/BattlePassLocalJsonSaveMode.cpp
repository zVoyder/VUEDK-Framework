// Copyright VUEDK, Inc. All Rights Reserved.

#include "SaveModes/BattlePassLocalJsonSaveMode.h"
#include "JsonObjectConverter.h"
#include "Data/BattlePassesSaveData.h"

void UBattlePassLocalJsonSaveMode::Save()
{
	Super::Save();
	FString JsonString;
	FJsonObjectConverter::UStructToJsonObjectString(CreateSaveData(), JsonString);
	const FString FilePath = SAVE_FILE_FULLPATH + TEXT(".json");
	FFileHelper::SaveStringToFile(JsonString, *FilePath);
}

void UBattlePassLocalJsonSaveMode::Load()
{
	Super::Load();

	const FString FilePath = SAVE_FILE_FULLPATH + TEXT(".json");
	FString JsonString;

	if (!FFileHelper::LoadFileToString(JsonString, *FilePath))
		return;
	
	FBattlePassesSaveData SaveData;
	FJsonObjectConverter::JsonObjectStringToUStruct(JsonString, &SaveData, 0, 0);
	CompleteLoad(SaveData);
}
