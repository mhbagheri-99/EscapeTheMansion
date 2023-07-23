// Fill out your copyright notice in the Description page of Project Settings.


#include "Grabber.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"

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

}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	UPhysicsHandleComponent* PhysicsHandleComponent = GetPhysicsHandle();
	if (PhysicsHandleComponent == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("No PhysicsHandleComponent Found!")); 
		return;
	}
	else
	{
		//Calculates the location of the grabbed object in front of the player with HoldDist distance
		FVector TargetLocation = GetComponentLocation() + GetForwardVector() * HoldDist;
		//Sets the object location to the new one calculated above and its rotation to its original rotation (when grabbed)
		PhysicsHandleComponent->SetTargetLocationAndRotation(TargetLocation, GetComponentRotation());
	}
}

void UGrabber::Grab()
{
	UPhysicsHandleComponent* PhysicsHandleComponent = GetPhysicsHandle();
	if (PhysicsHandleComponent == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("No PhysicsHandleComponent Found!"));
		return;
	}
	//Saves what we hit within our GrabRadius and MaxGrabDist
	FHitResult HitResult;

	if (GetGrabbableInReach(HitResult))
	{
		//Drawing a sphere in the Impactpoint of the hit objects
		//DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 10, 10, FColor::Blue, false, 5);

		UPrimitiveComponent* HitComponent = HitResult.GetComponent();
		//Enables the physics for the grabbed object and wakes it up if it is asleep
		HitComponent->SetSimulatePhysics(true);
		HitComponent->WakeAllRigidBodies();
		//Adds "Grabbed" tag to the component needed later to release it and possibly other things 
		//(like glowing the grabbed object and etc.)
		HitResult.GetActor()->Tags.Add("Grabbed");
		//Detaches the grabbable object if it is attached to any other components
		HitResult.GetActor()->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		//Grabs the component using the built in function of PhysicsHandleComponent
		PhysicsHandleComponent->GrabComponentAtLocationWithRotation(
			HitComponent,
			NAME_None,
			HitResult.ImpactPoint,
			GetComponentRotation()
		);
	}

}

void UGrabber::Release()
{
	UPhysicsHandleComponent* PhysicsHandleComponent = GetPhysicsHandle();
	if (PhysicsHandleComponent == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("No PhysicsHandleComponent Found!"));
		return;
	}
	else 
	{
		//If there is a grabbed component
		if(PhysicsHandleComponent->GetGrabbedComponent() != nullptr)
		{
			//Wakes it up if it is in hibernation
			PhysicsHandleComponent->GetGrabbedComponent()->WakeAllRigidBodies();
			//Removes the "Grabbed" tag from it
			PhysicsHandleComponent->GetGrabbedComponent()->GetOwner()->Tags.Remove("Grabbed");
			//Releases the component using the built in function of PhysicsHandleComponent
			PhysicsHandleComponent->ReleaseComponent();
		}
	}
}

//a const boolean that checks if the collision sphere hit something grabbable in our scene
//if true it means there is a grabbable object in our reach, otherwise returns false
//Also takes the HitResult as reference to save the hit object and return it
bool UGrabber::GetGrabbableInReach(FHitResult &HitResult) const
{
	FVector Start, End;

	Start = GetComponentLocation();
	End = Start + MaxGrabDist * GetForwardVector();

	//DrawDebugLine(GetWorld(), Start, End, FColor::Black);
	//DrawDebugSphere(GetWorld(), End, 10, 10, FColor::Green, false, 5);

	FCollisionShape CollisionSphere = FCollisionShape::MakeSphere(GrabRadius);
	//Should check "DefaultEngine.ini" to retrieve trace channel name
	return GetWorld()->SweepSingleByChannel(
		HitResult,
		Start, End,
		FQuat::Identity,
		ECC_GameTraceChannel2,
		CollisionSphere);
}

//Checks if the object has the PhysicsHandleComponent or not
//Returns nullptr if it doesn't and if it does returns the PhysicsHandleComponent
UPhysicsHandleComponent* UGrabber::GetPhysicsHandle() const
{
	UPhysicsHandleComponent* Result = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();

	if (Result == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("No PhysicsHandleComponent Found!"));
	}
	return Result;
}