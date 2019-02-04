// copy right bader fahoum2018

#include "Grabber.h"
#include "Engine/World.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Actor.h"
#include "Runtime/Engine/Classes/GameFramework/Actor.h"
#include "Components/PrimitiveComponent.h"
#include "DrawDebugHelpers.h"

#define OUT
// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

}

// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("Grabber reporting for Duty!"));
	FindPhysicsHandleComponent();
	SetupInputComponent();
	
}

void UGrabber::FindPhysicsHandleComponent()
{
	// look for attached Physics Handle
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (PhysicsHandle == nullptr) {
		// if physics handle not found 
		UE_LOG(LogTemp, Error, TEXT("%s missing physics handle component"), *(GetOwner()->GetName()));
	}
}

void UGrabber::Grab()
{
	// Ray-Cast and grab whats in reach
	UE_LOG(LogTemp, Warning, TEXT("GRAB pressed "));

	//Try and reach (LINE TRACE) any actors with physics body collision channel set
	auto HitResult = GetFirstPhysicsBodyInReach();
	auto ComponentToGrab = HitResult.GetComponent(); // gets the mesh in our case
	auto ActorHit = HitResult.GetActor();

	//If we hit something then attach a physics handle
	if (ActorHit) {
		// attach physics handle
		PhysicsHandle->GrabComponent(
			ComponentToGrab,
			NAME_None,
			ComponentToGrab->GetOwner()->GetActorLocation(),
			true
		);
		
	}
}

//called when Grab is released

inline void UGrabber::Release()
{
	UE_LOG(LogTemp, Warning, TEXT("Grab Released "));
	// release physics handle
	PhysicsHandle->ReleaseComponent();
}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	FVector LineTranceEnd = GetReachLineEnd();
	// if the physics handle is attached
	if (PhysicsHandle->GrabbedComponent) {
		// move the object we're holding
		PhysicsHandle->SetTargetLocation(LineTranceEnd);
	}
}

FVector UGrabber::GetReachLineEnd() const{
	///get player view point this tick 
	FVector	PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation
	);
	return PlayerViewPointLocation + (PlayerViewPointRotation.Vector() * Reach);
	
}

FVector UGrabber::GetReachLineStart() const{
	///get player view point this tick 
	FVector	PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation
	);
	return PlayerViewPointLocation;	
}


// Setup (assumed) attached input component
// Look for attached InputComponent

inline void UGrabber::SetupInputComponent()
{

	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputComponent) {
		// InputComponent found 
		//UE_LOG(LogTemp, Warning, TEXT("INPUT COMPONENT found"));
		// bind the input Action
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);

	}
	else {
		UE_LOG(LogTemp, Error, TEXT("%s missing INPUT Component"), *(GetOwner()->GetName()));
	}
}

FHitResult UGrabber::GetFirstPhysicsBodyInReach() const
{

	//debug line
	/*DrawDebugLine(
		GetWorld(),
		PlayerViewPointLocation,
		LineTranceEnd,
		FColor(255, 0, 0),
		false,
		0.f,
		0.f,
		10.f
	); */

	/// Line-trace (ray-cast) out to reach distance 
	FHitResult HitResult;
	///setup query paramertrs
	FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetOwner());
	GetWorld()->LineTraceSingleByObjectType(
		OUT HitResult,
		GetReachLineStart(),
		GetReachLineEnd(),
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParameters
	);
	/// see what we hit
	//AActor* ActorHit = Hit.GetActor();
	//if (ActorHit) {
	//	UE_LOG(LogTemp, Warning, TEXT("trace hit hitting on : %s"), *(ActorHit->GetName()));
	//}
	return HitResult;
}
