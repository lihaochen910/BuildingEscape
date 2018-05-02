// Fill out your copyright notice in the Description page of Project Settings.

#include "OpenDoor.h"


// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();
	
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (PressurePlate != nullptr)
	{
		if (this->GetMass() > 30.f)
		{
			OpenDoor();
		}
		else
		{
			CloseDoor();
		}
	}
}

float UOpenDoor::GetMass()
{
	float TotalMass = 0.f;
	TArray<AActor*> Array;

	PressurePlate->GetOverlappingActors(OUT Array);

	for (const auto& Actor : Array)
	{
		TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
		//UE_LOG(LogTemp, Warning, TEXT("%s mass: %f"), *Actor->GetName(), Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass());
	}

	return TotalMass;
}

void UOpenDoor::OpenDoor()
{
	AActor* Owner = GetOwner();

	FRotator NewRotation = FRotator(0, OpenAngle, 0);

	Owner->SetActorRotation(NewRotation);
}

void UOpenDoor::CloseDoor()
{
	AActor* Owner = GetOwner();

	FRotator NewRotation = FRotator(0, -180, 0);

	Owner->SetActorRotation(NewRotation);
}
