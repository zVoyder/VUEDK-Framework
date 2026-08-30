// Copyright VUEDK, Inc. All Rights Reserved.

#include "Squad/SquadHandler.h"
#include "Squad/Squad.h"

void USquadHandler::Init(USquad* InOwnerSquad)
{
	bIsActive = bAutoActivate;
	OwnerSquad = InOwnerSquad;
	OnInit(InOwnerSquad);
}

void USquadHandler::Deinit()
{
	bIsActive = false;
	OwnerSquad = nullptr;
}

void USquadHandler::SetActive(const bool bNewActive)
{
	bIsActive = bNewActive;

	if (bIsActive)
		BeginPlay();
	else
		EndPlay();
}

bool USquadHandler::IsActive() const
{
	return bIsActive;
}

USquad* USquadHandler::GetSquad() const
{
	return OwnerSquad;
}

void USquadHandler::Tick(const float DeltaTime)
{
	if (!bCanEverTick || !Check() || !IsActive())
		return;

	ReceiveTick(DeltaTime);
}

void USquadHandler::BeginPlay()
{
	if (!IsActive())
		return;

	ReceiveBeginPlay();
}

void USquadHandler::EndPlay()
{
	if (!IsActive())
		return;

	ReceiveEndPlay();
}

#if WITH_EDITOR
bool USquadHandler::ImplementsGetWorld() const
{
	return true;
}
#endif

UWorld* USquadHandler::GetWorld() const
{
	return IsValid(OwnerSquad) ? OwnerSquad->GetWorld() : nullptr;
}

void USquadHandler::OnRemovedFromSquad_Implementation(USquad* InOwnerSquad)
{
}

void USquadHandler::OnAddedToSquad_Implementation(USquad* InOwnerSquad)
{
}

void USquadHandler::OnInit_Implementation(USquad* InOwnerSquad)
{
}

bool USquadHandler::Check() const
{
	return IsValid(OwnerSquad) && IsValid(this);
}
