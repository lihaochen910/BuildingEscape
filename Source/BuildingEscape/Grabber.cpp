// Fill out your copyright notice in the Description page of Project Settings.

#include "Grabber.h"


// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();
	
	playerController = GetWorld()->GetFirstPlayerController();
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();

	TraceParameters = FCollisionQueryParams(FName(TEXT("")), false, GetOwner());
	ObjectQueryParams = FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody);

	if (PhysicsHandle)
		GEngine->AddOnScreenDebugMessage(0, 3, FColor::Green, TEXT("PhysicsHandle found"));

	if (InputComponent) {
		GEngine->AddOnScreenDebugMessage(0, 3, FColor::Green, TEXT("InputComponent found"));
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::CheckGrab);
	}

}


void UGrabber::Grab(FHitResult& result)
{
	if (result.GetActor() != nullptr)
	{
		PhysicsHandle->GrabComponent(result.GetComponent(), NAME_None, result.GetComponent()->GetOwner()->GetActorLocation(), true);

		GEngine->AddOnScreenDebugMessage(0, 3, FColor::Yellow, *FString::Printf(TEXT("Grabed %s"), *result.GetActor()->GetName()));

		grabState = GrabState::Grabing;
	}
}


void UGrabber::Release()
{
	if (PhysicsHandle->GrabbedComponent != nullptr)
	{
		GEngine->AddOnScreenDebugMessage(0, 3, FColor::Green, *FString::Printf(TEXT("Release %s"), *PhysicsHandle->GrabbedComponent->GetOwner()->GetName()));

		PhysicsHandle->ReleaseComponent();

		grabState = GrabState::Released;
	}
}


void UGrabber::CheckGrab()
{
	if (grabState == GrabState::Grabing)
	{
		Release();
	}
	else {
		FVector PlayerViewPointPosition;
		FRotator PlayerRotation;

		playerController->GetPlayerViewPoint(OUT PlayerViewPointPosition, OUT PlayerRotation);

		/*DrawDebugLine(GetWorld(), PlayerViewPointPosition, PlayerViewPointPosition + PlayerRotation.Vector() * RayLength,
		FColor::Red, false, 0,0, 10);*/

		DrawDebugLine(GetWorld(), GetOwner()->GetActorLocation(), PlayerViewPointPosition + PlayerRotation.Vector() * RayLength,
			FColor::Red, false, 0, 0, 1);

		FHitResult Hit;

		if (
			GetWorld()->LineTraceSingleByObjectType(
				OUT Hit, GetOwner()->GetActorLocation(), PlayerViewPointPosition + PlayerRotation.Vector() * RayLength,
				ObjectQueryParams, TraceParameters
			))
		{
			UE_LOG(LogTemp, Warning, TEXT("hit Object: %s"), *Hit.GetActor()->GetName());
			Grab(Hit);
		}
	}
	
}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (PhysicsHandle->GrabbedComponent)
	{
		FVector PlayerViewPointPosition;
		FRotator PlayerRotation;

		playerController->GetPlayerViewPoint(OUT PlayerViewPointPosition, OUT PlayerRotation);

		PhysicsHandle->SetTargetLocation(PlayerViewPointPosition + PlayerRotation.Vector() * RayLength);
	}
}
