// Fill out your copyright notice in the Description page of Project Settings.

#include "GrabberComp.h"
#include "GameFramework/PlayerController.h"
#include "DrawDebugHelpers.h"

#define OUT

// Sets default values for this component's properties
UGrabberComp::UGrabberComp()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGrabberComp::BeginPlay()
{
	Super::BeginPlay();

	FindPhysicsHandle();

	SetupInputComp();
	
	

}

void UGrabberComp::FindPhysicsHandle()
{
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if(!PhysicsHandle)
	{
		UE_LOG(LogTemp, Error, TEXT("No physics handle component found on %s"), *GetOwner()->GetName());
	}
}

//Sets up all of the Inputs
void UGrabberComp::SetupInputComp()
{
	InputComp = GetOwner()->FindComponentByClass<UInputComponent>();
	if(InputComp)
	{
		UE_LOG(LogTemp, Error, TEXT("Input component found!"));
		InputComp->BindAction("Grab", IE_Pressed, this, &UGrabberComp::Grab);
		InputComp->BindAction("Grab", IE_Released, this, &UGrabberComp::Release);
	}
}

// Called every frame
void UGrabberComp::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//If line trace finds an actor that could potentially be the pickable actor, then it does the thing, which I commented below
	if(!PhysicsHandle) {return;}
	if(PhysicsHandle->GrabbedComponent)
	{
		//Sets the location of grabbed actor, to move in along with the cursor (player or line trace end)
		PhysicsHandle->SetTargetLocation(GetCurrentLineTraceEnd());
	}


}

FHitResult UGrabberComp::LineTracingTheLookingActor() const
{
	//I'll leave it here, for debugging shit
	/* DrawDebugLine(
		GetWorld(),
		PlayerViewPointLocation,
		LineTraceEnd,
		FColor(0, 255, 0),
		false,
		0.f,
		0,
		5
	); */

	//Struct, that will contain info, about hitted actors by line
	FHitResult Hit;

	//QueryParams for line trace check. This is query params for line, exactly
	FCollisionQueryParams TraceParams(FName(TEXT("")), false, GetOwner());
	
	//Finally the ray casting itself. Definition is below.
	//Checking is actor pickable, by ray casting(line tracing)
	GetWorld()->LineTraceSingleByObjectType(
		OUT Hit,
		GetPlayersWorldPos(),
		GetCurrentLineTraceEnd(),
		//And this query params are used for object collision channel type
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParams
	);
	return Hit;
}

void UGrabberComp::Grab()
{
	//For debug
	//UE_LOG(LogTemp, Warning, TEXT("Grabber Pressed"));

	//Struct, that will contain info, about hitted actors by line
	FHitResult HitResult = LineTracingTheLookingActor();

	//Component of hitted actor, which were hitted too(some kinda mesh)
	UPrimitiveComponent* ComponentToGrab = HitResult.GetComponent();

	//Attach the physics handle to hitted actor
	AActor* HittedActor = HitResult.GetActor();
	if(HittedActor)
	{
		if(!PhysicsHandle) {return;}
		PhysicsHandle->GrabComponentAtLocation
		(
			ComponentToGrab,
			NAME_None,
			GetCurrentLineTraceEnd()
		);
	}
}

void UGrabberComp::Release()
{
	UE_LOG(LogTemp, Warning, TEXT("Grabber Released"));
	if(!PhysicsHandle) {return;}
	PhysicsHandle->ReleaseComponent();
}

FVector UGrabberComp::GetCurrentLineTraceEnd() const
{
	//This gets a player view point, for doing line trace check. It will be the start of the line, actually
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation
	);
	//Until here

	//Finding were the ray cast (line trace) checking line will end.
	return PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;
}

FVector UGrabberComp::GetPlayersWorldPos() const
{
	//This gets a player view point, for doing line trace check. It will be the start of the line, actually
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation
	);
	//Until here

	return PlayerViewPointLocation;
}