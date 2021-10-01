// Fill out your copyright notice in the Description page of Project Settings.


#include "RoguePickupHealth.h"

#include "RogueAttributeComponent.h"

void ARoguePickupHealth::Interact_Implementation(APawn* InstigatorPawn) {
	if (!IsActive) {
		return;
	}
	URogueAttributeComponent* AttributeComponent = Cast<URogueAttributeComponent>(InstigatorPawn->GetComponentByClass(URogueAttributeComponent::StaticClass()));
	bool IsHealed = AttributeComponent->ApplyHealthChange(20.f);
	
	if (IsHealed) {
		DisableItem();
		GetWorldTimerManager().SetTimer(EnableItemTimerHandle, this, &ARoguePickupHealth::EnableItem, 10.f);
	}	
}

void ARoguePickupHealth::DisableItem() {
	StaticMeshComponent->SetVisibility(false);
	IsActive = false;
}

void ARoguePickupHealth::EnableItem() {
	StaticMeshComponent->SetVisibility(true);
	IsActive = true;
}
