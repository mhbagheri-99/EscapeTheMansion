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
		FVector TargetLocation = GetComponentLocation() + GetForwardVector() * HoldDist;

		PhysicsHandleComponent->SetTargetLocationAndRotation(TargetLocation, GetComponentRotation());
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
		if(PhysicsHandleComponent->GetGrabbedComponent() != nullptr)
		{
			PhysicsHandleComponent->GetGrabbedComponent()->WakeAllRigidBodies();
			PhysicsHandleComponent->GetGrabbedComponent()->GetOwner()->Tags.Remove("Grabbed");
			PhysicsHandleComponent->ReleaseComponent();
		}
	}
}

bool UGrabber::GetGrabbableInReach(FHitResult &HitResult) const
{
	FVector Start, End;

	Start = GetComponentLocation();
	End = Start + MaxGrabDist * GetForwardVector();

	DrawDebugLine(GetWorld(), Start, End, FColor::Black);
	DrawDebugSphere(GetWorld(), End, 10, 10, FColor::Green, false, 5);

	FCollisionShape CollisionSphere = FCollisionShape::MakeSphere(GrabRadius);
	return GetWorld()->SweepSingleByChannel(
		HitResult,
		Start, End,
		FQuat::Identity,
		ECC_GameTraceChannel2,
		CollisionSphere);
}

void UGrabber::Grab()
{
	UPhysicsHandleComponent* PhysicsHandleComponent = GetPhysicsHandle();
	if (PhysicsHandleComponent == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("No PhysicsHandleComponent Found!"));
		return;
	}

	FHitResult HitResult;
	
	if (GetGrabbableInReach(HitResult))
	{
		DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 10, 10, FColor::Blue, false, 5);

		UPrimitiveComponent* HitComponent = HitResult.GetComponent();
		HitComponent->SetSimulatePhysics(true);
		HitComponent->WakeAllRigidBodies();
		HitResult.GetActor()->Tags.Add("Grabbed");
		HitResult.GetActor()->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		PhysicsHandleComponent->GrabComponentAtLocationWithRotation(
			HitComponent,
			NAME_None,
			HitResult.ImpactPoint,
			GetComponentRotation()
			);
	}

}

UPhysicsHandleComponent* UGrabber::GetPhysicsHandle() const
{
	UPhysicsHandleComponent* Result = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();

	if (Result == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("No PhysicsHandleComponent Found!"));
	}
	return Result;
}