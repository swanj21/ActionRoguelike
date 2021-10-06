// Fill out your copyright notice in the Description page of Project Settings.


#include "RogueAttributeComponent.h"

// Sets default values for this component's properties
URogueAttributeComponent::URogueAttributeComponent() {}

bool URogueAttributeComponent::ApplyHealthChange(AActor* InstigatorActor, float Delta) {
	if (!GetOwner()->CanBeDamaged()) {
		return false;
	}
	
	if (Health == MaxHealth && Delta > 0) {
		return false;
	}
	Health += Delta;

	Health = FMath::Clamp(Health, 0.f, MaxHealth);

	OnHealthChanged.Broadcast(InstigatorActor, this, Health, Delta);
	return true;
}

bool URogueAttributeComponent::IsAlive() const {
	return Health > 0.f;
}

bool URogueAttributeComponent::IsLowHealth() const {
	return Health <= LowHealthThreshold;
}

bool URogueAttributeComponent::Kill(AActor* InstigatorActor) {
	return ApplyHealthChange(InstigatorActor, -GetMaxHealth());
}

// ----------------
// Static functions
// ----------------
URogueAttributeComponent* URogueAttributeComponent::GetAttributes(AActor* FromActor) {
	if (FromActor) {
		return Cast<URogueAttributeComponent>(FromActor->GetComponentByClass(URogueAttributeComponent::StaticClass()));
	}
	return nullptr;
}

bool URogueAttributeComponent::IsActorAlive(AActor* Actor) {
	if (Actor) {
		URogueAttributeComponent* AttributeComponent = GetAttributes(Actor);
		if (AttributeComponent) {
			return AttributeComponent->IsAlive();
		}
	}
	return false;
}
