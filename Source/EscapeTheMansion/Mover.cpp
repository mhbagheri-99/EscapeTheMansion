#include "Mover.h"
#include "Math/UnrealMathUtility.h"

// Sets default values for this component's properties
UMover::UMover()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UMover::BeginPlay()
{
	Super::BeginPlay();
	
	//Saves the start location
	OriginalLocation = GetOwner()->GetActorLocation();
}


// Called every frame
void UMover::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	//Sets the TargetLocation to its OriginalLocation
	FVector TargetLocation = OriginalLocation;

	//If it should move
	if (ShouldMove)
	{
		//Sets the TargetLocation MoveOffset amount from its OriginalLocation
		TargetLocation = OriginalLocation + MoveOffset;
	}
	//Gets the CurrentLocation of the actor
	FVector CurrentLocation = GetOwner()->GetActorLocation();
	//Sets the speed based on the MoveOffset amount and MoveTime
	float Speed = MoveOffset.Length() / MoveTime;
	//Calculates the NewLocation with VInterpConstantTo function (It is like lerp in bluprint)
	FVector NewLocation = FMath::VInterpConstantTo(CurrentLocation, TargetLocation, DeltaTime, Speed);
	//Sets the actor location to the new one
	GetOwner()->SetActorLocation(NewLocation);
}

//If you don't put this if it might cause the engine to crash
void UMover::SetShouldMove(bool Triggered)
{
	if (this != nullptr)
		ShouldMove = Triggered;
}

bool UMover::GetShouldMove() 
{
	return ShouldMove;
}

