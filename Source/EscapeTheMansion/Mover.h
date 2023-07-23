#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Mover.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ESCAPETHEMANSION_API UMover : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMover();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//(De)Activates the component
	void SetShouldMove(bool Triggered);
	
	UFUNCTION(BlueprintCallable)
		bool GetShouldMove();

private:
	//How much it should move
	UPROPERTY(EditAnywhere)
	FVector MoveOffset;

	//How long it should take to move
	UPROPERTY(EditAnywhere)
	float MoveTime = 4.0;

	//Activation state
	UPROPERTY(EditAnywhere)
	bool ShouldMove = false;

	//Original starting location
	FVector OriginalLocation;
		
};
