// Fill out your copyright notice in the Description page of Project Settings.


#include "TriggerComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UTriggerComponent::UTriggerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

}

void UTriggerComponent::BeginPlay()
{
	Super::BeginPlay();

}

void UTriggerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	//Gets the triggering actor
	AActor* Actor = GetTriggeringActor();

	if (Actor == nullptr)
	{	
		//XOR Triggered and false so if they are different ChangedState would be true otherwise if both are the same it would be false
		ChangedState = Triggered ^ false;
		Triggered = false;
		//Mover activation is the same as Triggered variable (e.g. if not triggered it should not move)
		Mover->SetShouldMove(Triggered);
		UE_LOG(LogTemp, Warning, TEXT("No appropriate actor detected."));
	}
	else
	{	
		//This section was not useful in my use case but it would disable the physics of the triggering actor
		//when it has triggered the mechanism, it also attaches it to the triggerable component so if they move,
		//they can move together smoothly
		/*
		UPrimitiveComponent* Component = Cast<UPrimitiveComponent>(Actor->GetRootComponent());
		if (Component != nullptr)
		{
			Component->SetSimulatePhysics(false);
		}
		Actor->AttachToComponent(this, FAttachmentTransformRules::KeepWorldTransform);
		*/

		//XOR Triggered and true so if they are different ChangedState would be true otherwise if both are the same it would be false
		ChangedState = Triggered ^ true;
		Triggered = true;
		//Mover activation is the same as Triggered variable (e.g. if not triggered it should not move)
		Mover->SetShouldMove(Triggered);
		UE_LOG(LogTemp, Warning, TEXT("Triggered."));
	}

}

//If you don't put this if it might cause the engine to crash
void UTriggerComponent::SetMover(UMover* NewMover)
{
	if (this != nullptr)
	{
		Mover = NewMover;
	}
}

bool UTriggerComponent::GetTriggered()
{
	return Triggered;
}

bool UTriggerComponent::GetChangedState()
{
	return ChangedState;
}

//Gets the overlapping actors and checks if they can trigger the component or not based on their tag
//Returns nullptr if there is no triggering overlap otherwise returns the triggering actor
AActor* UTriggerComponent::GetTriggeringActor() const
{
	TArray<AActor*> Actors;
	GetOverlappingActors(Actors);

	if (Actors.Num() > 0)
	{
		for (AActor* Actor : Actors)
		{
			if (Actor->ActorHasTag(TriggeringActorTag))
			{
				return Actor;
			}
		}
	}
	return nullptr;
}
