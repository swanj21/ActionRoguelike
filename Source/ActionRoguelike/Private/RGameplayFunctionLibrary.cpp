// Fill out your copyright notice in the Description page of Project Settings.


#include "RGameplayFunctionLibrary.h"

#include "RogueAttributeComponent.h"
#include "RogueCharacter.h"
#include "RPlayerState.h"


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

bool URGameplayFunctionLibrary::GiveCredits(AController* To, float Amount/*, UEnum::CreditReason Reason */) {
	return ModifyCredits(To, Amount);
}

bool URGameplayFunctionLibrary::TakeCredits(AController* From, float Amount/*, UEnum::CreditReason Reason */) {
	return ModifyCredits(From, -Amount);
}

bool URGameplayFunctionLibrary::ModifyCredits(AController* Controller, float Amount) {
	UE_LOG(LogTemp, Warning, TEXT("Modifying %f credits on %s"), Amount, *GetNameSafe(Controller))
	if (!Controller) {
		return false;
	}
	ARPlayerState* PlayerState = Controller->GetPlayerState<ARPlayerState>();
	if (!PlayerState) {
		return false;
	}
	return PlayerState->UpdateCredits(Amount);
}