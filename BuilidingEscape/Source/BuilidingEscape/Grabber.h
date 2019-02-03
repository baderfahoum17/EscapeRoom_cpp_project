// copy right bader fahoum2018

#pragma once
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Components/InputComponent.h"
#include "Grabber.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILIDINGESCAPE_API UGrabber : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGrabber();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


private:
	// how far ahead of the player can we reach in cm
	float Reach = 100.f;
	// PHYSICS handler component 
	UPhysicsHandleComponent* PhysicsHandle = nullptr;
	UInputComponent* InputComponent = nullptr;
	
	//Ray-cast and grab whats in reach
	void Grab();

	//called when Grab is released
	void Release();

	//find attached physics handle 
	void FindPhysicsHandleComponent();

	// Setup (assumed) attached input component
	// Look for attached InputComponent
	void SetupInputComponent();
	
	// Return hit for first physics body in reach
	FHitResult GetFirstPhysicsBodyInReach() const;

};
