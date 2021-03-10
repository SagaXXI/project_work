// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "GrabberComp.generated.h"



UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ESCAPE_API UGrabberComp : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGrabberComp();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;


private:

	UPROPERTY(EditAnywhere)
	float Reach = 100.f;

	//To rotate the objects like in skyrim
	UPROPERTY()
	UPhysicsHandleComponent* PhysicsHandle = nullptr;

	//To interact with players devices, pressing any key etc.
	UPROPERTY()
	UInputComponent* InputComp = nullptr;

	//Grab the line traced and pickable actor
	void Grab();
	//Releases current grabbed actor
	void Release();

	void FindPhysicsHandle();

	void SetupInputComp();

	//Does the line trace check
	FHitResult LineTracingTheLookingActor() const;

	//Calculates and returns current line trace endpoint
	FVector GetCurrentLineTraceEnd() const;
	
	//Calculates and retruns players location in world
	FVector GetPlayersWorldPos() const;

};
