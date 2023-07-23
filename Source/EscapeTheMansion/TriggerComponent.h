// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "Mover.h"
#include "TriggerComponent.generated.h"


/**
 * 
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class ESCAPETHEMANSION_API UTriggerComponent : public UBoxComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UTriggerComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//Sets the mover component of the movable/triggered object to control it
	UFUNCTION(BlueprintCallable)
		void SetMover(UMover* Mover);

	//A boolean that shows if the mechanism is triggered or not
	UFUNCTION(BlueprintCallable)
		bool GetTriggered();

	//A boolean that would be true if and only if there is a change in the triggered state of the mechanism
	//Useful for playing sound effects such as opening the door
	UFUNCTION(BlueprintCallable)
		bool GetChangedState();


private:
	//TriggeringActorTag is the tag of the object(s) that can trigger the mechanism
	UPROPERTY(EditAnywhere)
		FName TriggeringActorTag = "";

	UMover* Mover;

	bool Triggered = false;
	bool ChangedState = false;

	AActor* GetTriggeringActor() const;

};
