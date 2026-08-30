// Copyright VUEDK, Inc. All Rights Reserved.

#include "Senses/Sense_Damage.h"
#include "SenseValidators/SenseValidator_IsA.h"
#include "Sensors/Sensor_Damage.h"

USense_Damage::USense_Damage()
{
	DefaultSensedStatus = ESensedStatus::Memory;
	USensor_Damage* DamageSensor = CreateDefaultSubobject<USensor_Damage>(TEXT("DamageSensor"));
	Sensors.Add(DamageSensor);
	USenseValidator_IsA* DamageValidator = CreateDefaultSubobject<USenseValidator_IsA>(TEXT("DamageValidator"));
	DamageValidator->ActorClass = APawn::StaticClass();
	Validators.Add(DamageValidator);
}
