// Fill out your copyright notice in the Description page of Project Settings.


#include "RogueInteractionComponent.h"
#include "RogueGameplayInterface.h"
#include "DrawDebugHelpers.h"

// Sets default values for this component's properties
URogueInteractionComponent::URogueInteractionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void URogueInteractionComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void URogueInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void URogueInteractionComponent::PrimaryInteract() {
	FHitResult hitResult;
	FCollisionObjectQueryParams objectQueryParams(ECC_WorldDynamic);

	AActor* owner = GetOwner();
	if (!owner) {
		UE_LOG(LogTemp, Fatal, TEXT("Interaction failed, owner of InteractionComponent not found"))
		return;
	}

	FVector eyeLocation;
	FRotator eyeRotation;

	owner->GetActorEyesViewPoint(eyeLocation, eyeRotation);

	FVector end = eyeLocation + (eyeRotation.Vector() * 1000);
	
	// bool blockingHit = GetWorld()->LineTraceSingleByObjectType(hitResult, eyeLocation, end, objectQueryParams);

	// Sphere tracing
	TArray<FHitResult> hits;
	FCollisionShape shape;
	float sphereRadius = 30.f;
	shape.SetSphere(sphereRadius);

	bool blockingHit = GetWorld()->SweepMultiByObjectType(hits, eyeLocation, end, FQuat::Identity, objectQueryParams, shape);

	FColor lineColor = blockingHit ? FColor::Blue : FColor::Red;
	
	for (FHitResult hit : hits) {
		AActor* hitActor = hit.GetActor();
		if (hitActor && hitActor->Implements<URogueGameplayInterface>()) {
			IRogueGameplayInterface::Execute_Interact(hitActor, Cast<APawn>(owner));
		}
		DrawDebugSphere(GetWorld(), hit.ImpactPoint, sphereRadius, 32, lineColor, false, 2.f);
		break;
	}

	DrawDebugLine(GetWorld(), eyeLocation, end, lineColor, false, 5.f, 0, 2.f);
}
