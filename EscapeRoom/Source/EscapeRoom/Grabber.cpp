// Fill out your copyright notice in the Description page of Project Settings.

#include "Grabber.h"
#include "EscapeRoom.h"
#include "Components/PrimitiveComponent.h"
#include "DrawDebugHelpers.h"
#include "GameFrameWork/PlayerController.h"
#include "Engine/World.h"

#define OUT
// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	FindPhysicSHandleComponent();
	SetupInputComponent();
}

void UGrabber::Grab() 
{
	UE_LOG(LogTemp, Warning, TEXT("Grab key pressed"));

	//LINE TRACE and reach any actors with physics body colliion channel set
	auto HitResult = GetFirstPhysicsBodyInReach();
	auto ComponentToGrab = HitResult.GetComponent();
	auto ActorHit = HitResult.GetActor();
	//If we hit something then attach a physics handle 
	if (ActorHit) 
	{
		//TODO attach physics handle
		PhysicsHandle->GrabComponent(ComponentToGrab, NAME_None, ComponentToGrab->GetOwner()->GetActorLocation(), true);
	}

}
void UGrabber::Release()
{
	UE_LOG(LogTemp, Warning, TEXT("Grab key released"));
	PhysicsHandle->ReleaseComponent();
}

void UGrabber::FindPhysicSHandleComponent()
{
	//Look for attached Physics Handle component
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (PhysicsHandle)
	{
		//Physics handle is found
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("Physics Handle component is missing on %s!\n"), *GetOwner()->GetName());
	}
}

void UGrabber::SetupInputComponent()
{
	//Look for attached Input component
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputComponent) //Bind Actions
	{
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("Input component is missing on %s!\n"), *GetOwner()->GetName());
	}
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	/// Get player view point this tick
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation);

	//UE_LOG(LogTemp, Warning, TEXT("Location: %s, Rotation : %s"), *PlayerViewPointLocation.ToString(), *PlayerViewPointRotation.ToString());

	//Draw a red trace in the world

	FVector LineTraceEnd = PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;
	// if the physics handle is attached
	if (PhysicsHandle->GrabbedComponent) {
		//move the object that we're holding
		PhysicsHandle->SetTargetLocation(LineTraceEnd);
	}
}

const FHitResult UGrabber::GetFirstPhysicsBodyInReach()
{
	/// Get player view point this tick
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation);

	//UE_LOG(LogTemp, Warning, TEXT("Location: %s, Rotation : %s"), *PlayerViewPointLocation.ToString(), *PlayerViewPointRotation.ToString());

	//Draw a red trace in the world

	FVector LineTraceEnd = PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;
	///Setup query params
	FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetOwner());
	/// Ray-cast out to reach distance
	FHitResult Hit;
	GetWorld()->LineTraceSingleByObjectType(OUT Hit, PlayerViewPointLocation, LineTraceEnd, FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody), TraceParameters);

	AActor* ActorHit = Hit.GetActor();
	if (ActorHit) {
		/// See what we hit
		//UE_LOG(LogTemp, Warning, TEXT("Line trace hit: %s"), *(ActorHit->GetName()));
	}

	return Hit;
}


