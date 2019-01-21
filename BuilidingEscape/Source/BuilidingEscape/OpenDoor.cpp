// copy right bader fahoum2018

#include "OpenDoor.h"
#include "GameFramework/Actor.h"
#include "Runtime/Engine/Classes/GameFramework/Actor.h"

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
	
	
}

void UOpenDoor::OpenDoor()
{
	// Find the owning Actor
	AActor* DoorOwner = GetOwner();
	// create a rotator 
	FRotator NewRotation = FRotator(0.0f, -60.0f, 0.0f);
	// Set the door rotation 
	DoorOwner->SetActorRotation(NewRotation);
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Poll the Trigger Volume
	if (PressurePlate->IsOverlappingActor(ActorThatOpens)) {
		// If the ActorThatOpens is in the volume
		OpenDoor();
	}
}

