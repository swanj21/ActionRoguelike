// Fill out your copyright notice in the Description page of Project Settings.


#include "RoguePickupHealth.h"

#include "RogueAttributeComponent.h"

void ARoguePickupHealth::Interact_Implementation(APawn* InstigatorPawn) {
	if (!IsActive) {
		return;
	}
	URogueAttributeComponent* AttributeComponent = Cast<URogueAttributeComponent>(InstigatorPawn->GetComponentByClass(URogueAttributeComponent::StaticClass()));
	bool IsHealed = AttributeComponent->ApplyHealthChange(this, HealValue);
	
	if (IsHealed) {
		DisableItem();
		GetWorldTimerManager().SetTimer(EnableItemTimerHandle, this, &ARoguePickupHealth::EnableItem, RespawnTimer);
	}	
}

void ARoguePickupHealth::DisableItem() {
	// Could also do RootComponent->SetVisibility(false, true) where the 'true' tells it to propagate to all children.
	StaticMeshComponent->SetVisibility(false);
	IsActive = false;
}

void ARoguePickupHealth::EnableItem() {
	StaticMeshComponent->SetVisibility(true);
	IsActive = true;
}
