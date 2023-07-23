// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Grabber.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ESCAPETHEMANSION_API UGrabber : public USceneComponent
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
	
	UFUNCTION(BlueprintCallable)
		void Release();

	UFUNCTION(BlueprintCallable)
		void Grab();

private:
	//Max distance that you can grab an object from
	UPROPERTY(EditAnywhere)
		float MaxGrabDist = 160;

	//Distance between the player and the grabbed object
	UPROPERTY(EditAnywhere)
		float HoldDist = 80;
	
	//The size of the sphere that can trigger the seizable object (the bigger the less precision 
	//in grabbing objects close to each other)
	UPROPERTY(EditAnywhere)
		float GrabRadius = 50;

	UPhysicsHandleComponent* GetPhysicsHandle() const;

	bool GetGrabbableInReach(FHitResult& HitResult) const;

};
