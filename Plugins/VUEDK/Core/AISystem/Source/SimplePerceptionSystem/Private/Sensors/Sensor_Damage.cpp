// Copyright VUEDK, Inc. All Rights Reserved.

#include "Sensors/Sensor_Damage.h"

void USensor_Damage::Init(USimplePerceiver* InSimplePerceiver)
{
	Super::Init(InSimplePerceiver);

	AActor* OwnerActor = GetOwner();
	if (!IsValid(OwnerActor))
		return;

	OwnerActor->OnTakeAnyDamage.AddDynamic(this, &USensor_Damage::OnTakeAnyDamage);
}

void USensor_Damage::OnTakeAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
{
	if (IsValid(InstigatedBy))
	{
		SenseActor(InstigatedBy->GetPawn());
		return;
	}

	SenseActor(DamageCauser);
}
