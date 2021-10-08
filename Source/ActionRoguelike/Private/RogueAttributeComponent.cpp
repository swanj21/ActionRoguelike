// Fill out your copyright notice in the Description page of Project Settings.


#include "RogueAttributeComponent.h"

#include "RogueGameModeBase.h"

static TAutoConsoleVariable<float> CVarDamageModifier(TEXT("rogue.DamageMultiplier"), 1.f, TEXT("Global damage modifier for attribute component"), ECVF_Cheat);

// Sets default values for this component's properties
URogueAttributeComponent::URogueAttributeComponent() {}

// -------------------- //
// ------ HEALTH ------ //
// -------------------- //
bool URogueAttributeComponent::ApplyHealthChange(AActor* InstigatorActor, float Delta) {
	if (!GetOwner()->CanBeDamaged() && Delta < 0.f) {
		return false;
	}
	
	if (Health == MaxHealth && Delta > 0.f) {
		return false;
	}

	if (Delta < 0.f) {
		float DamageMultiplier = CVarDamageModifier.GetValueOnGameThread();

		Delta *= DamageMultiplier;
	}
	float OldHealth = Health;
	
	Health += Delta;

	Health = FMath::Clamp(Health, 0.f, MaxHealth);

	float ActualDelta = Health - OldHealth;

	OnHealthChanged.Broadcast(InstigatorActor, this, Health, ActualDelta);

	// Died
	if (ActualDelta < 0.f && Health <= 0.f) {
		ARogueGameModeBase* GameMode = GetWorld()->GetAuthGameMode<ARogueGameModeBase>();
		if (GameMode) {
			GameMode->OnActorKilled(GetOwner(), InstigatorActor);
		}
	}
	
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
