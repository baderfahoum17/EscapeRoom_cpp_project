// copy right bader fahoum2018
#include "OpenDoor.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "Components/PrimitiveComponent.h"
#include "GameFramework/Actor.h"
#include "Runtime/Engine/Classes/GameFramework/Actor.h"
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
	DoorOwner = GetOwner();
	
}

void UOpenDoor::OpenDoor()
{
	// Find the owning Actor
	// AActor* DoorOwner = GetOwner();
	// create a rotator 
	// FRotator NewRotation = FRotator(0.0f, -60.0f, 0.0f);
	// Set the door rotation 
	DoorOwner->SetActorRotation(FRotator(0.0f, OpenAngle, 0.0f));
}
void UOpenDoor::CloseDoor()
{
	// Find the owning Actor
	// AActor* DoorOwner = GetOwner();
	// create a rotator 
	// FRotator NewRotation = FRotator(0.0f, 0.0f, 0.0f);
	// Set the door rotation 
	DoorOwner->SetActorRotation(FRotator(0.0f, 0.0f, 0.0f));
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Poll the Trigger Volume
	if (GetTotalMassOfActorsOnPlate() > 50.f) // TODO make int a paramter
	{
		OpenDoor();
		LastDoorOpenTime = GetWorld()->GetTimeSeconds();
	}
	
	// check if its time to close door
	if (GetWorld()->GetTimeSeconds() - LastDoorOpenTime > DoorCloseDelay) {
		CloseDoor();
	}
}

float UOpenDoor::GetTotalMassOfActorsOnPlate()
{
	float TotalMass = 0.f;
	// find all the overlapping actors
	TArray<AActor*> OverlappingActors;
	PressurePlate->GetOverlappingActors(OUT OverlappingActors);
	//Iterate through them addin their masses
	for (AActor* Actor : OverlappingActors) {
		Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
		UE_LOG(LogTemp, Warning, TEXT("%s on pressue plate"), *Actor->GetName());
		TotalMass = TotalMass + Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
	}

	return TotalMass;
}

