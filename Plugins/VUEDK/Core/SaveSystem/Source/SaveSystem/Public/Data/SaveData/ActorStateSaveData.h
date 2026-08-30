// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "SaveDataBase.h"
#include "Components/Savers/Behaviours/ActorStateSaveBehaviour.h"
#include "ActorStateSaveData.generated.h"

namespace ActorSaveKeys
{
	static const FName Location = "Loc";
	static const FName Rotation = "Rot";
	static const FName Scale    = "Scale";
	static const FName LinearVelocity = "LinVel";
	static const FName AngularVelocity = "AngVel";
	static const FName Mass = "Mass";
}

UCLASS()
class SAVESYSTEM_API UActorStateSaveData : public USaveDataBase
{
	GENERATED_BODY()

public:
	UPROPERTY(SaveGame)
	TMap<FName, FVector> SavedVectors;
	UPROPERTY(SaveGame)
	TMap<FName, float> SavedValues;

private:
	UPROPERTY()
	UActorStateSaveBehaviour* Behaviour = nullptr;

public:
	void Init(UActorStateSaveBehaviour* InBehaviour);
	
	virtual bool SaveObjectData(UObject* ObjectToSave) override;

	virtual bool LoadObjectData(UObject* ObjectToLoad) override;

private:
	void SaveLocation(const FVector& InLocation);

	void SaveRotation(const FRotator& InRotation);

	void SaveScale(const FVector& InScale);

	void SaveLinearVelocity(const FVector& InVelocity);

	void SaveAngularVelocity(const FVector& InVelocity);
	
	void SaveMass(float InMass);

	FVector GetLocation() const;

	FRotator GetRotation() const;

	FVector GetScale() const;

	FVector GetLinearVelocity() const;

	FVector GetAngularVelocity() const;

	float GetMass() const;

	bool HasLocation() const;
	
	bool HasRotation() const;
	
	bool HasScale() const;
	
	bool HasLinearVelocity() const;
	
	bool HasAngularVelocity() const;

	bool HasMass() const;
	
	bool Check() const;
};
