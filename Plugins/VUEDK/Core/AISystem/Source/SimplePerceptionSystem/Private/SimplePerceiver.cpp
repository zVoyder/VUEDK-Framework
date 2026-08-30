// Copyright VUEDK, Inc. All Rights Reserved.

#include "SimplePerceiver.h"
#include "SimplePerceptionSystem.h"
#include "Factories/SimplePerceptionFactory.h"
#include "Senses/Base/SenseBase.h"

USimplePerceiver::USimplePerceiver()
{
	PrimaryComponentTick.bCanEverTick = true;
	UActorComponent::SetAutoActivate(true);
}

void USimplePerceiver::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!IsActive())
		return;

	ProcessSenses(DeltaTime);
	UpdatePerceivedActors();
}

void USimplePerceiver::Deactivate()
{
	Super::Deactivate();
	ClearPerception();
}

void USimplePerceiver::ClearPerception()
{
	PerceivedActors.Empty();
	OnClearPerception.Broadcast();
}

void USimplePerceiver::PerceiveActor(AActor* Actor, const ESensedStatus SensedStatus)
{
	if (!IsValid(Actor))
		return;

	const UWorld* World = GetWorld();
	if (!IsValid(World))
		return;

	const FSensedActorInfo SensedActorInfo = FSensedActorInfo(nullptr, World->GetTimeSeconds(), Actor->GetActorTransform(), SensedStatus);
	PerceivedActors.Add(Actor, SensedActorInfo);
	OnPerceivedActor.Broadcast(Actor, SensedActorInfo);
	OnPerceivedNewActor.Broadcast(Actor, SensedActorInfo);
}

void USimplePerceiver::SenseActor(AActor* Actor, USenseBase* SensedBy)
{
	const UWorld* World = GetWorld();
	if (!IsValid(World))
		return;

	const TEnumAsByte<ESensedStatus> TargetStatus = SensedBy->DefaultSensedStatus;
	if (PerceivedActors.Contains(Actor))
	{
		FSensedActorInfo& SensedActorInfo = PerceivedActors[Actor];
		SensedActorInfo.SensedBy = SensedBy;
		SensedActorInfo.SensedTime = World->GetTimeSeconds();
		SensedActorInfo.SensedTransform = Actor->GetActorTransform();
		if (SensedActorInfo.SensedStatus != TargetStatus)
		{
			SensedActorInfo.SensedStatus = TargetStatus;
			OnPerceivedActor.Broadcast(Actor, SensedActorInfo);
		}

		return;
	}

	const FSensedActorInfo SensedActorInfo = FSensedActorInfo(SensedBy, World->GetTimeSeconds(), Actor->GetActorTransform(), TargetStatus);
	PerceivedActors.Add(Actor, SensedActorInfo);
	OnPerceivedActor.Broadcast(Actor, SensedActorInfo);
	OnPerceivedNewActor.Broadcast(Actor, SensedActorInfo);
}

bool USimplePerceiver::IsPerceived(AActor* Actor) const
{
	return PerceivedActors.Contains(Actor);
}

FSensedActorInfo USimplePerceiver::GetPerceivedActorInfo(AActor* Actor) const
{
	if (const FSensedActorInfo* SensedActorInfo = PerceivedActors.Find(Actor))
		return *SensedActorInfo;

	return FSensedActorInfo();
}

TMap<AActor*, FSensedActorInfo> USimplePerceiver::GetPerceivedActors() const
{
	TMap<AActor*, FSensedActorInfo> Result;

	for (const auto& Pair : PerceivedActors)
	{
		if (AActor* Actor = Pair.Key.Get())
			Result.Add(Actor, Pair.Value);
	}

	Result.ValueSort([](const FSensedActorInfo& A, const FSensedActorInfo& B)
	{
		if (A.SensedStatus != B.SensedStatus)
			return A.SensedStatus < B.SensedStatus;

		return A.SensedTime > B.SensedTime;
	});
	
	return Result;
}

void USimplePerceiver::BeginPlay()
{
	Super::BeginPlay();
	SetupSenses();

#if !UE_BUILD_SHIPPING
	DrawDebugSensedActors();
#endif
}

void USimplePerceiver::SetupSenses()
{
	if (!Check())
	{
		UE_LOG(LogSimplePerceptionSystem, Warning, TEXT("USimplePerceiver::SetupSenses: Check failed for SimplePerceiver %s."), *GetOwner()->GetName());
		return;
	}
	
	for (const USenseBase* SenseTemplate : PerceptionConfigData->Senses)
	{
		USenseBase* NewSense = USimplePerceptionFactory::CreateSense(SenseTemplate, this);
		Senses.Add(NewSense);
	}
}

void USimplePerceiver::ProcessSenses(const float DeltaTime)
{
	for (USenseBase* Sense : Senses)
	{
		if (!IsValid(Sense))
			continue;

		Sense->Process(DeltaTime);
	}
}

void USimplePerceiver::UpdatePerceivedActors()
{
	const UWorld* World = GetWorld();
	if (!IsValid(World))
		return;

	for (auto It = PerceivedActors.CreateIterator(); It; ++It)
	{
		AActor* Actor = It->Key.Get();
		FSensedActorInfo& SensedActorInfo = It->Value;

		if (!IsValid(Actor)) // Invalid cleanup
		{
			It.RemoveCurrent();
			continue;
		}

		// Update the SensedActor
		if (SensedActorInfo.SensedStatus == ESensedStatus::Direct)
		{
			const USenseBase* Sense = SensedActorInfo.SensedBy.Get();
			if (!IsValid(Sense) || !Sense->ValidateSensedActor(Actor))
			{
				SensedActorInfo.SensedStatus = ESensedStatus::Memory;
				OnPerceivedActorLost.Broadcast(Actor, SensedActorInfo);
			}
		}

		if (SensedActorInfo.SensedStatus == ESensedStatus::Memory)
		{
			const float CurrentTime = World->GetTimeSeconds();
			const float TimeSinceLastSensed = CurrentTime - SensedActorInfo.SensedTime;
			if (TimeSinceLastSensed > PerceptionConfigData->MemoryDuration)
			{
				SensedActorInfo.SensedStatus = ESensedStatus::None;
				OnPerceivedActorForgotten.Broadcast(Actor, SensedActorInfo);
				It.RemoveCurrent();
			}
		}
	}
}

bool USimplePerceiver::Check() const
{
	return IsValid(PerceptionConfigData);
}

#if !UE_BUILD_SHIPPING
void USimplePerceiver::DrawDebugSensedActors() const
{
	const bool bDebug = FSimplePerceptionSystemModule::CVarDebugPerceptionSystem.GetValueOnGameThread();

	const UWorld* World = GetWorld();
	if (!IsValid(World))
		return;

	World->GetTimerManager().SetTimerForNextTick(this, &USimplePerceiver::DrawDebugSensedActors);
	if (!bDebug || !IsActive())
		return;

	AActor* OwnerActor = GetOwner();
	if (!IsValid(OwnerActor))
		return;

	AActor* Owner = OwnerActor->GetClass()->IsChildOf(AController::StaticClass()) ? Cast<AController>(OwnerActor)->GetPawn() : OwnerActor;
	if (!IsValid(Owner) || Owner->IsHidden())
		return;

	FString DebugString;
	// Header
	DebugString += TEXT("[ PERCEIVED ACTORS ]\n");
	DebugString += TEXT("===============================\n");

	for (const auto& SensedActor : PerceivedActors)
	{
		const AActor* Actor = SensedActor.Key.Get();
		const FSensedActorInfo& SensedActorInfo = SensedActor.Value;
		if (!IsValid(Actor))
			continue;

		const FVector DrawSphereLocation = SensedActorInfo.SensedTransform.GetLocation();
		DrawDebugSphere(
			World,
			DrawSphereLocation,
			30.0f,
			12,
			SensedActorInfo.SensedStatus == ESensedStatus::Direct ? FColor::Green : FColor::Yellow,
			false,
			0.f,
			0,
			2.0f
		);

		DebugString += TEXT("-------------------------------\n");

		DebugString += FString::Printf(
			TEXT("  Actor : %s\n"),
			*Actor->GetName()
		);

		DebugString += FString::Printf(
			TEXT("  Sense : %s\n"),
			IsValid(SensedActorInfo.SensedBy) ? *SensedActorInfo.SensedBy->GetName() : TEXT("None")
		);

		DebugString += FString::Printf(
			TEXT("  Status : %s\n"),
			*UEnum::GetValueAsString(SensedActorInfo.SensedStatus)
		);

		if (SensedActorInfo.SensedStatus == ESensedStatus::Memory)
		{
			DebugString += FString::Printf(
				TEXT("  Memory Time : %.2f/%.2f\n"),
				World->GetTimeSeconds() - SensedActorInfo.SensedTime,
				PerceptionConfigData->MemoryDuration
			);
		}

		DebugString += TEXT("-------------------------------\n");
	}

	DebugString += TEXT("===============================");
	DrawDebugString(
		World,
		FVector(0.f, 0.f, 120.f), // Offset above actor
		DebugString,
		Owner,
		FColor::White,
		0.f,
		true,
		0.8f
	);
}
#endif
