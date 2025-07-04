// Copyright VUEDK, Inc. All Rights Reserved.

#include "Attributes/ResourceAttribute.h"
#include "ResourceAttributesManager.h"
#include "ResourceAttributesSystem.h"

void UResourceAttribute::Init(UResourceAttributeData* InResourceAttributeData, UResourceAttributesManager* InResourceAttributesManager)
{
	if (!IsValid(InResourceAttributeData))
	{
		UE_LOG(LogResourceAttributesSystem, Error, TEXT("UResourceAttribute::Init: Invalid ResourceAttributeData provided."));
		return;
	}

	if (!IsValid(InResourceAttributesManager))
	{
		UE_LOG(LogResourceAttributesSystem, Error, TEXT("UResourceAttribute::Init: Invalid ResourceAttributesManager provided."));
		return;
	}

	ResourceAttributeData = InResourceAttributeData;
	ResourceAttributesManager = InResourceAttributesManager;
	SetMinValue(ResourceAttributeData->DefaultMinValue);
	SetMaxValue(ResourceAttributeData->DefaultMaxValue);
	SetRegenerationRate(ResourceAttributeData->DefaultRegenerationRate);

	CurrentValue = FMath::Clamp(ResourceAttributeData->StartingValue, MinValue, MaxValue);
	if (ResourceAttributeData->bHasRegeneration && ResourceAttributeData->bRegenOnInit)
		StartRegen(ResourceAttributeData->RegenerationDelayOnInit);
}

void UResourceAttribute::Tick(float DeltaTime)
{
	ProcessRegen(DeltaTime);
}

bool UResourceAttribute::IsTickable() const
{
	if (!Check())
		return false;

	return ResourceAttributeData->bHasRegeneration && bIsRegenerating;
}

TStatId UResourceAttribute::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(UResourceAttribute, STATGROUP_Tickables);
}

bool UResourceAttribute::IsAtMin() const
{
	return CurrentValue <= MinValue;
}

bool UResourceAttribute::IsAtMax() const
{
	return CurrentValue >= MaxValue;
}

float UResourceAttribute::GetValue() const
{
	return CurrentValue;
}

float UResourceAttribute::GetValueAsPercent() const
{
	const float Percent = (CurrentValue - MinValue) / (MaxValue - MinValue);
	return Percent;
}

float UResourceAttribute::GetMinValue() const
{
	return MinValue;
}

float UResourceAttribute::GetMaxValue() const
{
	return MaxValue;
}

float UResourceAttribute::GetRegenerationRate() const
{
	return RegenerationRate;
}

void UResourceAttribute::SetValue(const float NewValue)
{
	const float OldValue = CurrentValue;
	CurrentValue = FMath::Clamp(NewValue, MinValue, MaxValue);

	if (OldValue == CurrentValue)
		return;

	OnResourceAttributeChanged.Broadcast(OldValue, CurrentValue);

	if (CurrentValue > OldValue)
		OnAttributeIncreased(OldValue);
	else
		OnAttributeDecreased(OldValue);

	if (IsAtMin())
		OnResourceAttributeReachedMinValue.Broadcast(CurrentValue);

	if (IsAtMax())
		OnResourceAttributeReachedMaxValue.Broadcast(CurrentValue);
}

void UResourceAttribute::ModifyValue(const float Amount)
{
	const float NewValue = CurrentValue + Amount;
	SetValue(NewValue);
}

void UResourceAttribute::DrainToMin()
{
	SetValue(MinValue);
}

void UResourceAttribute::FillToMax()
{
	SetValue(MaxValue);
}

void UResourceAttribute::SetMinValue(const float NewMinValue)
{
	const float OldValue = MinValue;
	MinValue = FMath::Clamp(NewMinValue, FLT_MIN, MaxValue);
	OnResourceAttributeMinValueChanged.Broadcast(OldValue, MinValue);

	if (CurrentValue < MinValue)
		SetValue(MinValue);
}

void UResourceAttribute::SetMaxValue(const float NewMaxValue)
{
	const float OldValue = MaxValue;
	MaxValue = FMath::Clamp(NewMaxValue, MinValue, FLT_MAX);
	OnResourceAttributeMaxValueChanged.Broadcast(OldValue, MaxValue);

	if (CurrentValue > MaxValue)
		SetValue(MaxValue);
}

void UResourceAttribute::SetRegenerationRate(const float NewRegenerationRate)
{
	RegenerationRate = FMath::Clamp(NewRegenerationRate, 0.0f, FLT_MAX);
}

void UResourceAttribute::StartRegen(const float Delay)
{
	if (!Check())
	{
		UE_LOG(LogResourceAttributesSystem, Error, TEXT("UResourceAttribute::StartRegen: ResourceAttributeData is not valid."));
		return;
	}

	if (!ResourceAttributeData->bHasRegeneration)
		return;

	bIsRegenerating = false;

	const UWorld* World = ResourceAttributesManager->GetWorld();
	if (!IsValid(World))
	{
		UE_LOG(LogResourceAttributesSystem, Error, TEXT("UResourceAttribute::StartRegen: World is not valid."));
		return;
	}

	if (Delay <= 0.0f)
	{
		OnRegenStarted();
		return;
	}

	FTimerManager& TimerManager = World->GetTimerManager();
	TimerManager.ClearTimer(RegenTimerHandle);
	TimerManager.SetTimer(
		RegenTimerHandle,
		this,
		&UResourceAttribute::OnRegenStarted,
		Delay
	);
}

void UResourceAttribute::StopRegen()
{
	if (!Check())
	{
		UE_LOG(LogResourceAttributesSystem, Error, TEXT("UResourceAttribute::StopRegen: ResourceAttributeData is not valid."));
		return;
	}

	if (!ResourceAttributeData->bHasRegeneration)
		return;

	bIsRegenerating = false;

	const UWorld* World = ResourceAttributesManager->GetWorld();
	if (!IsValid(World))
	{
		UE_LOG(LogResourceAttributesSystem, Error, TEXT("UResourceAttribute::StopRegen: World is not valid."));
		return;
	}

	FTimerManager& TimerManager = World->GetTimerManager();
	TimerManager.ClearTimer(RegenTimerHandle);
	OnResourceAttributeRegenStopped.Broadcast(CurrentValue);
}

void UResourceAttribute::ProcessRegen(const float DeltaTime)
{
	const float OldValue = CurrentValue;
	const float AddValue = RegenerationRate * DeltaTime;
	CurrentValue = FMath::Clamp(CurrentValue + AddValue, MinValue, MaxValue);

	OnResourceAttributeChanged.Broadcast(OldValue, CurrentValue);
	OnResourceAttributeIncreased.Broadcast(CurrentValue - OldValue, OldValue, CurrentValue);

	if (IsAtMax())
		StopRegen();
}

void UResourceAttribute::OnAttributeIncreased(const float OldValue)
{
	if (!Check())
		return;

	if (ResourceAttributeData->bRegenAfterIncrement)
		StartRegen(ResourceAttributeData->RegenerationDelayAfterIncrement);

	 
}

void UResourceAttribute::OnAttributeDecreased(const float OldValue)
{
	if (!Check())
		return;

	if (ResourceAttributeData->bRegenAfterDecrement)
		StartRegen(ResourceAttributeData->RegenerationDelayAfterDecrement);

	OnResourceAttributeDecreased.Broadcast(OldValue - CurrentValue, OldValue, CurrentValue);
}

void UResourceAttribute::OnRegenStarted()
{
	bIsRegenerating = true;
	OnResourceAttributeRegenStarted.Broadcast(CurrentValue);
}

bool UResourceAttribute::Check() const
{
	return IsValid(ResourceAttributeData) && IsValid(ResourceAttributesManager);
}
