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
	AActor* Actor = GetTriggeringActor();

	if (Actor == nullptr)
	{	
		ChangedState = Triggered ^ false;
		Triggered = false;
		Mover->SetShouldMove(Triggered);
		UE_LOG(LogTemp, Warning, TEXT("No appropriate actor detected."));
	}
	else
	{	
		/*
		UPrimitiveComponent* Component = Cast<UPrimitiveComponent>(Actor->GetRootComponent());
		if (Component != nullptr)
		{
			Component->SetSimulatePhysics(false);
		}
		Actor->AttachToComponent(this, FAttachmentTransformRules::KeepWorldTransform);
		*/
		ChangedState = Triggered ^ true;
		Triggered = true;
		Mover->SetShouldMove(Triggered);
		UE_LOG(LogTemp, Warning, TEXT("Triggered."));
	}

}

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
