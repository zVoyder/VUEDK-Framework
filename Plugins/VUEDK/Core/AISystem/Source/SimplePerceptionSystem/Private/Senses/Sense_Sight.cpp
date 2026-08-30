// Copyright VUEDK, Inc. All Rights Reserved.

#include "Senses/Sense_Sight.h"
#include "SenseValidators/SenseValidator_Distance.h"
#include "SenseValidators/SenseValidator_Dot.h"
#include "SenseValidators/SenseValidator_LineOfSight.h"
#include "Sensors/Sensor_SphereTrace.h"

USense_Sight::USense_Sight()
{
	USensor_SphereTrace* SphereTraceSensor = CreateDefaultSubobject<USensor_SphereTrace>(TEXT("SphereTrace"));
	Sensors.Add(SphereTraceSensor);
	USenseValidator_Distance* DistanceValidator = CreateDefaultSubobject<USenseValidator_Distance>(TEXT("DistanceValidator"));
	Validators.Add(DistanceValidator);
	USenseValidator_Dot* DotValidator = CreateDefaultSubobject<USenseValidator_Dot>(TEXT("DotValidator"));
	Validators.Add(DotValidator);
	USenseValidator_LineOfSight* LineOfSightValidator = CreateDefaultSubobject<USenseValidator_LineOfSight>(TEXT("LineOfSightValidator"));
	Validators.Add(LineOfSightValidator);
}
