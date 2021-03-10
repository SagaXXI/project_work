// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/TriggerVolume.h"
#include "Components/AudioComponent.h"
#include "OpenDoor.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ESCAPE_API UOpenDoor : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UOpenDoor();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void OpenDoor(float DeltaTime);
	void CloseDoor(float DeltaTime);
	void FindAudioComp();

	//Checks PressurePlate for nullptr
	void CheckPressurePlate();
	
	//To protect of playing the sound multiple times, because it is in a tick function
	bool CloseDoorSoundPlayed = false;
	bool OpenDoorSoundPlayed = false;

private:
	float InitialYaw;
	float CurrentYaw;
	
	float DoorLastTimeOpened = 0.f;
	
	UPROPERTY(EditAnywhere)
	float DoorCloseDelay = .5f;

	UPROPERTY(EditAnywhere)
	float DoorOpenSpeed = .8f;

	UPROPERTY(EditAnywhere)
	float DoorCloseSpeed = 2.f;
	
	UPROPERTY(EditAnywhere)
	float TargetYaw = 90.f;

	UPROPERTY(EditAnywhere)
	float MassToOpenDoor = 50.f;

	//Trigger Volume for openning the door
	UPROPERTY(EditAnywhere)
	ATriggerVolume* PressurePlate = nullptr;

	//Finds and calculates the mass of all of the currently overlapped actors(for checking of pressure plate)
	float TotalMassOfActors() const;
	
	//Plays the sound, when door is opening or closing
	UPROPERTY()
	UAudioComponent* AudioComp = nullptr;

	
};
