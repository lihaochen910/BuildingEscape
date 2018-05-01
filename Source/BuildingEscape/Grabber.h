// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Core.h"
#include "Components/ActorComponent.h"
#include "Runtime/Engine/Public/DrawDebugHelpers.h"
#include "Runtime/Engine/Public/CollisionQueryParams.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Runtime/Engine/Classes/Components/InputComponent.h"
#include "Grabber.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UGrabber : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGrabber();

	enum class GrabState {
		Grabing,
		Released
	};

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	GrabState grabState = GrabState::Released;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void CheckGrab();
	void Grab(FHitResult& result);
	void Release();
		
	UPROPERTY(EditAnywhere)
		float RayLength = 9.0f;

private:

	APlayerController* playerController;

	UPhysicsHandleComponent* PhysicsHandle;

	UInputComponent* InputComponent;

	FCollisionQueryParams TraceParameters;
	FCollisionObjectQueryParams ObjectQueryParams;
};
