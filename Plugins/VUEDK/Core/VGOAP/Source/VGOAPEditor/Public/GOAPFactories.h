// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Factories/Factory.h"
#include "GOAPFactories.generated.h"

UCLASS()
class VGOAPEDITOR_API UGOAPBehaviourFactory : public UFactory
{
	GENERATED_BODY()

public:
	UGOAPBehaviourFactory();

	virtual UObject* FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn) override;

	virtual FName GetNewAssetThumbnailOverride() const override;
};

UCLASS()
class VGOAPEDITOR_API UGOAPBlackboardFactory : public UFactory
{
	GENERATED_BODY()

public:
	UGOAPBlackboardFactory();

	virtual UObject* FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn) override;

	virtual FName GetNewAssetThumbnailOverride() const override;
};

UCLASS()
class VGOAPEDITOR_API UGOAPBeliefFactory : public UFactory
{
	GENERATED_BODY()

public:
	UGOAPBeliefFactory();

	virtual UObject* FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn) override;
};

UCLASS()
class VGOAPEDITOR_API UGOAPActionFactory : public UFactory
{
	GENERATED_BODY()

public:
	UGOAPActionFactory();

	virtual UObject* FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn) override;
};

UCLASS()
class VGOAPEDITOR_API UGOAPGoalFactory : public UFactory
{
	GENERATED_BODY()

public:
	UGOAPGoalFactory();

	virtual UObject* FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn) override;
};

UCLASS()
class VGOAPEDITOR_API UGOAPSensorFactory : public UFactory
{
	GENERATED_BODY()

public:
	UGOAPSensorFactory();

	virtual UObject* FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn) override;
};
