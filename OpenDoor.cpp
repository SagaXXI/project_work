// Fill out your copyright notice in the Description page of Project Settings.


#include "OpenDoor.h"
#include "GameFramework/Actor.h"
#include "Math.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "TimerManager.h"
#include "Components/PrimitiveComponent.h"
#include "Components/AudioComponent.h"

#define OUT
// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();

	InitialYaw = GetOwner()->GetActorRotation().Yaw;
	CurrentYaw = InitialYaw;
	TargetYaw += InitialYaw;

	CheckPressurePlate();
	FindAudioComp();

}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	if(TotalMassOfActors() >= MassToOpenDoor)
	{
		OpenDoor(DeltaTime);
		//Otchet nachalsya, on nuzhen dlya zakritia dvery, to est' CloseDoor()
		DoorLastTimeOpened = GetWorld()->GetTimeSeconds();
	}
	else
	{
		if(GetWorld()->GetTimeSeconds() - DoorLastTimeOpened > DoorCloseDelay /*|| TotalMassOfActors() == 0*/)
		{
			CloseDoor(DeltaTime);
		}
	}
	
	

	// ...
}

void UOpenDoor::OpenDoor(float DeltaTime)
{
	//For debugging
	// UE_LOG(LogTemp, Warning, TEXT("%s"), *GetOwner()->GetActorRotation().ToString());
	// UE_LOG(LogTemp, Warning, TEXT("Yaw is %f"), GetOwner()->GetActorRotation().Yaw);

	CurrentYaw = FMath::Lerp(CurrentYaw, TargetYaw, DeltaTime * DoorOpenSpeed);
	FRotator DoorRotation = GetOwner()->GetActorRotation();	
	DoorRotation.Yaw = CurrentYaw;
	GetOwner()->SetActorRotation(DoorRotation);

	CloseDoorSoundPlayed = false;
	if(!AudioComp) {return;}
	if(!OpenDoorSoundPlayed)
	{
		AudioComp->Play();
		OpenDoorSoundPlayed = true;
	}
	
}

//Same as OpenDoor(), but changed TargetYaw, to the Yaw, which was in the beginningdg, where the door was shut
void UOpenDoor::CloseDoor(float DeltaTime)
{
	CurrentYaw = FMath::Lerp(CurrentYaw, InitialYaw, DeltaTime * DoorCloseSpeed);
	FRotator DoorRotation = GetOwner()->GetActorRotation();	
	DoorRotation.Yaw = CurrentYaw;
	GetOwner()->SetActorRotation(DoorRotation);

	OpenDoorSoundPlayed = false;
	if(!AudioComp) {return;}
	if(!CloseDoorSoundPlayed)
	{
		AudioComp->Play();
		CloseDoorSoundPlayed = true;
	}
}

float UOpenDoor::TotalMassOfActors() const
{
	float TotalMass = 0.f;

	//Find all overlapped actors
	TArray <AActor*> OverlappingActors;
	if(!PressurePlate) {return TotalMass;}
	PressurePlate->GetOverlappingActors(OUT OverlappingActors);

	//Add up all of the masses of actors
	//This for loop is especially good for iterating in maps, arrays, and etc.
	for (AActor* Actor : OverlappingActors)
	{
		TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
	}
	return TotalMass;
}

void UOpenDoor::FindAudioComp()
{
	AudioComp = GetOwner()->FindComponentByClass<UAudioComponent>();
	if(!AudioComp)
	{
		UE_LOG(LogTemp, Error, TEXT("%s is missing audio component!"), *GetOwner()->GetName());
	}
}

void UOpenDoor::CheckPressurePlate()
{
	if(!PressurePlate)
	{
		UE_LOG(LogTemp, Error, TEXT("%s has the open door component on it, but no PressurePlate set!"), *GetOwner()->GetName());
	}
}