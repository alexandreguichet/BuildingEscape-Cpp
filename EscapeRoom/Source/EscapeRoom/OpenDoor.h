// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/TriggerVolume.h"
#include "OpenDoor.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ESCAPEROOM_API UOpenDoor : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UOpenDoor();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	void OpenDoor();
	void CloseDoor();


public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	UPROPERTY(EditAnywhere) //Allows us to make this private variable visible in the Editor
	float OpenAngle = -90.0f;

	UPROPERTY(EditAnywhere) //Allows us to make this private variable editable in the Editor
	ATriggerVolume* PressurePlate;

	UPROPERTY(EditAnywhere) //Allows us to make this private variable editable in the Editor
	float DoorCloseDelay = 1.f;

	float LastDoorOpenTime = 0.f;

	AActor* ActorThatOpens; //Pawn inherits from actor
	AActor* Owner; //the owningDoor;
};

