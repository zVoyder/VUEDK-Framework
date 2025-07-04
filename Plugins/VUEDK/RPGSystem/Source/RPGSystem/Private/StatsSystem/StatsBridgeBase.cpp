// Copyright VUEDK, Inc. All Rights Reserved.

#include "StatsSystem/StatsBridgeBase.h"
#include "Factories/RPGFactory.h"
#include "StatsSystem/Data/SaveData/StatsBridgeSaveData.h"

UStatsBridgeBase::UStatsBridgeBase(): SpecialStatsContainer(nullptr),
                                      CoreStatsContainer(nullptr),
                                      FullStatsContainer(nullptr)
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UStatsBridgeBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	SpecialStatsContainer->OnStatsValuesChanged.RemoveDynamic(this, &UStatsBridgeBase::CalculateAllStatsValues);
	CoreStatsContainer->OnStatsValuesChanged.RemoveDynamic(this, &UStatsBridgeBase::CalculateFullStatsValues);
}

USaveData* UStatsBridgeBase::CreateSaveData()
{
	UStatsBridgeSaveData* BridgeSaveData = NewObject<UStatsBridgeSaveData>();

	for (auto& Pair : SpecialStatsContainer->GetValues())
		BridgeSaveData->SavedSpecialStats.Add(Pair.Key->StatID, Pair.Value);
	
	for (auto& Pair : CoreStatsContainer->GetValues())
		BridgeSaveData->SavedCoreStats.Add(Pair.Key->StatID, Pair.Value);
	
	return BridgeSaveData;
}

bool UStatsBridgeBase::LoadSaveData(USaveData* SavedData)
{
	if (!SavedData)
		return false;

	const UStatsBridgeSaveData* BridgeSaveData = Cast<UStatsBridgeSaveData>(SavedData);
	
	if (!BridgeSaveData)
		return false;

	for (auto& SpecialStatsMap = BridgeSaveData->SavedSpecialStats; auto& Pair : SpecialStatsMap)
	{
		if (USpecialStatData* SpecialStatData = GetSpecialStatByID(Pair.Key))
			SpecialStatsContainer->AddStat(SpecialStatData, Pair.Value);
	}
	
	for (auto& CoreStatsMap = BridgeSaveData->SavedCoreStats; auto& Pair : CoreStatsMap)
	{
		if (UCoreStatData* CoreStatData = GetCoreStatByID(Pair.Key))
			CoreStatsContainer->AddStat(CoreStatData, Pair.Value);
	}
	
	return true;
}

USpecialStatData* UStatsBridgeBase::GetSpecialStatByID(const FGuid SpecialStatID) const
{
	for (USpecialStatData* SpecialStat : SpecialStats)
	{
		if (SpecialStat->StatID == SpecialStatID)
			return SpecialStat;
	}

	return nullptr;
}

UCoreStatData* UStatsBridgeBase::GetCoreStatByID(const FGuid CoreStatID) const
{
	for (UCoreStatData* CoreStatData : CoreStats)
	{
		if (CoreStatData->StatID == CoreStatID)
			return CoreStatData;
	}

	return nullptr;
}

USpecialStatData* UStatsBridgeBase::GetSpecialStatByCodeName(const FString& SpecialStatCodeName) const
{
	for (USpecialStatData* SpecialStat : SpecialStats)
	{
		if (SpecialStat->StatCodeName == SpecialStatCodeName)
			return SpecialStat;
	}

	return nullptr;
}

UCoreStatData* UStatsBridgeBase::GetCoreStatByCodeName(const FString& CoreStatCodeName) const
{
	for (UCoreStatData* CoreStatData : CoreStats)
	{
		if (CoreStatData->StatCodeName == CoreStatCodeName)
			return CoreStatData;
	}

	return nullptr;
}

TMap<UCoreStatData*, float> UStatsBridgeBase::GetFullStatsValues() const
{
	return FullStatsContainer->GetCoreStatsValues();
}

UCoreStatsContainer* UStatsBridgeBase::GetFullStatsContainer() const
{
	return FullStatsContainer;
}

float UStatsBridgeBase::GetFullStatValueAsFloat(const UStatDataBase* Stat) const
{
	return FullStatsContainer->GetValueAsFloat(Stat);
}

int32 UStatsBridgeBase::GetFullStatValueAsInt(const UStatDataBase* Stat) const
{
	return FullStatsContainer->GetValueAsInt(Stat);
}

FString UStatsBridgeBase::GetFullStatValueAsString(const UStatDataBase* Stat) const
{
	return FullStatsContainer->GetValueAsString(Stat);
}

int32 UStatsBridgeBase::GetFullStatsLength() const
{
	return FullStatsContainer->GetStatsLength();
}

void UStatsBridgeBase::CalculateCoreStatValueWithSpecialStat(USpecialStatData* InSpecialStat, UCoreStatData* CoreStatData, const TSubclassOf<UBridgeStatOperation> OperationClass)
{
	if (!IsValid(InSpecialStat) || !IsValid(CoreStatData))
		return;
	
	const UBridgeStatOperation* NewOperation = URPGFactory::CreateBridgeStatOperation(this, CoreStatData, InSpecialStat, OperationClass);
	
	if (!IsValid(NewOperation))
		return;

	CoreStatsContainer->TrySetValue(CoreStatData, NewOperation->GetResultOperation(), false);
}

void UStatsBridgeBase::CalculateAllStatsValues()
{
	CalculateCoreStatsValues();
	CalculateFullStatsValues();
}

void UStatsBridgeBase::BeginPlay()
{
	CreateStatsContainers();
	SpecialStatsContainer->AddSpecialStats(SpecialStats);
	CoreStatsContainer->AddCoreStats(CoreStats);
	CalculateAllStatsValues();
	Super::BeginPlay();
	SpecialStatsContainer->OnStatsValuesChanged.AddDynamic(this, &UStatsBridgeBase::CalculateAllStatsValues);
	CoreStatsContainer->OnStatsValuesChanged.AddDynamic(this, &UStatsBridgeBase::CalculateFullStatsValues);
}

void UStatsBridgeBase::SetFullStatValue(UStatDataBase* Stat, const float Value) const
{
	FullStatsContainer->TrySetValue(Stat, Value);
}

void UStatsBridgeBase::ModifyFullStatValue(UStatDataBase* Stat, const float Value) const
{
	FullStatsContainer->TryModifyValue(Stat, Value);
}

void UStatsBridgeBase::CalculateCoreStatsValues()
{
	NativeOnCalculateCoreStatsValues();
	OnCalculateCoreStatsValues();
	OnCalculatedCoreStatsValues.Broadcast(this);
}

void UStatsBridgeBase::CalculateFullStatsValues()
{
	FullStatsContainer->CopyStats(CoreStatsContainer); // By default, full values are equal to core values
	NativeOnCalculateFullStatsValues();
	OnCalculateFullStatsValues();
	OnCalculatedFullStatsValues.Broadcast(this);
}

void UStatsBridgeBase::NativeOnCalculateCoreStatsValues()
{
}

void UStatsBridgeBase::OnCalculateCoreStatsValues_Implementation()
{
}

void UStatsBridgeBase::NativeOnCalculateFullStatsValues()
{
}

void UStatsBridgeBase::OnCalculateFullStatsValues_Implementation()
{
}

void UStatsBridgeBase::CreateStatsContainers()
{
	SpecialStatsContainer = NewObject<USpecialStatsContainer>(this, TEXT("SpecialStatsContainer"));
	CoreStatsContainer = NewObject<UCoreStatsContainer>(this, TEXT("CoreStatsContainer"));
	FullStatsContainer = NewObject<UCoreStatsContainer>(this, TEXT("FullStatsContainer"));
}
