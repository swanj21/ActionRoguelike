// Fill out your copyright notice in the Description page of Project Settings.


#include "RAction_BlackHole.h"

#include "RogueAttributeComponent.h"

URAction_BlackHole::URAction_BlackHole() {
	RageCost = 20;
}

bool URAction_BlackHole::CanStart_Implementation(AActor* Instigator) {
	// If we can't start because of some reason in the base class, exit out early
	if (!Super::CanStart_Implementation(Instigator)) {
		return false;
	}

	URogueAttributeComponent* AttributeComponent = URogueAttributeComponent::GetAttributes(Instigator);

	return AttributeComponent->GetRage() >= RageCost;
}

void URAction_BlackHole::StartAction_Implementation(AActor* Instigator) {
	if (ensure(Instigator)) {
		Super::StartAction_Implementation(Instigator);
		
		URogueAttributeComponent* AttributeComponent = URogueAttributeComponent::GetAttributes(Instigator);
		AttributeComponent->ApplyRageChange(Instigator, -RageCost);
	}
}