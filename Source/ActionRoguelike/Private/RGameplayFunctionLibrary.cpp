// Fill out your copyright notice in the Description page of Project Settings.


#include "RGameplayFunctionLibrary.h"

#include "RogueAttributeComponent.h"


bool URGameplayFunctionLibrary::ApplyDamage(AActor* DamageCauser, AActor* TargetActor, float DamageAmount) {
	URogueAttributeComponent* AttributeComponent = URogueAttributeComponent::GetAttributes(TargetActor);
	if (AttributeComponent) {
		return AttributeComponent->ApplyHealthChange(DamageCauser, -DamageAmount);
	}
	return false;
}

bool URGameplayFunctionLibrary::ApplyDirectionalDamage(AActor* DamageCauser, AActor* TargetActor, float DamageAmount,
	const FHitResult& HitResult) {
	if (ApplyDamage(DamageCauser, TargetActor, DamageAmount)) {
		UPrimitiveComponent* HitComponent = HitResult.GetComponent();
		if (HitComponent && HitComponent->IsSimulatingPhysics(HitResult.BoneName)) {
			HitComponent->AddImpulseAtLocation(-HitResult.ImpactNormal * 300000.f, HitResult.ImpactPoint, HitResult.BoneName);
			return true;
		}
	}
	return false;
}
