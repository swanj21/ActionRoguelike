// Fill out your copyright notice in the Description page of Project Settings.


#include "RogueInteractionComponent.h"
#include "RogueGameplayInterface.h"
#include "DrawDebugHelpers.h"
#include "Blueprint/UserWidget.h"
#include "RWorldUserWidget.h"

static TAutoConsoleVariable<bool> CVarDebugDrawInteraction(
	TEXT("rogue.InteractionDebugDraw"), false, TEXT("Enable debug lines for Interaction component"), ECVF_Cheat);

URogueInteractionComponent::URogueInteractionComponent() {
	PrimaryComponentTick.bCanEverTick = true;

	TraceDistance = 500.f;
	TraceRadius = 30.f;
	ChannelToQuery = ECC_WorldDynamic;
}

void URogueInteractionComponent::BeginPlay() {
	Super::BeginPlay();
}

void URogueInteractionComponent::FindBestInteractable() {
	bool bDebugDraw = CVarDebugDrawInteraction.GetValueOnGameThread();

	FCollisionObjectQueryParams objectQueryParams(ChannelToQuery);

	AActor* owner = GetOwner();
	if (!owner) {
		UE_LOG(LogTemp, Fatal, TEXT("Interaction failed, owner of InteractionComponent not found"))
		return;
	}

	FVector eyeLocation;
	FRotator eyeRotation;

	owner->GetActorEyesViewPoint(eyeLocation, eyeRotation);

	FVector TraceEnd = eyeLocation + (eyeRotation.Vector() * TraceDistance);

	// bool blockingHit = GetWorld()->LineTraceSingleByObjectType(hitResult, eyeLocation, end, objectQueryParams);

	// Sphere tracing
	TArray<FHitResult> hits;
	FCollisionShape shape;
	shape.SetSphere(TraceRadius);

	bool blockingHit = GetWorld()->SweepMultiByObjectType(hits, eyeLocation, TraceEnd, FQuat::Identity,
	                                                      objectQueryParams, shape);

	FColor lineColor = blockingHit ? FColor::Blue : FColor::Red;

	FocusedActor = nullptr; // Clear the focus before trying to fill it
	for (FHitResult hit : hits) {
		if (bDebugDraw) {
			DrawDebugSphere(GetWorld(), hit.ImpactPoint, TraceRadius, 32, lineColor, false, 2.f);
		}

		AActor* HitActor = hit.GetActor();
		if (HitActor && HitActor->Implements<URogueGameplayInterface>()) {
			FocusedActor = HitActor;
		}
		break;
	}

	if (FocusedActor) {
		if (DefaultWidgetInstance == nullptr && ensure(DefaultWidgetClass)) {
			DefaultWidgetInstance = CreateWidget<URWorldUserWidget>(GetWorld(), DefaultWidgetClass);
		}

		if (DefaultWidgetInstance) {
			DefaultWidgetInstance->AttachedActor = FocusedActor;
			if (!DefaultWidgetInstance->IsInViewport()) {
				DefaultWidgetInstance->AddToViewport();
			}
		}
	} else {
		if (DefaultWidgetInstance) {
			DefaultWidgetInstance->RemoveFromParent();
		}
	}

	if (bDebugDraw) {
		DrawDebugLine(GetWorld(), eyeLocation, TraceEnd, lineColor, false, 5.f, 0, 2.f);
	}
}


// Called every frame
void URogueInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                               FActorComponentTickFunction* ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	APawn* LocalPawn = Cast<APawn>(GetOwner());
	if (LocalPawn->IsLocallyControlled()) {
		FindBestInteractable();
	}
}

void URogueInteractionComponent::PrimaryInteract() {
	ServerInteract(FocusedActor);
}

void URogueInteractionComponent::ServerInteract_Implementation(AActor* ActorInFocus) {
	if (ActorInFocus) {
		IRogueGameplayInterface::Execute_Interact(ActorInFocus, Cast<APawn>(GetOwner()));
	} else {
		UE_LOG(LogTemp, Error, TEXT("No focused actor to interact with"))
	}
}