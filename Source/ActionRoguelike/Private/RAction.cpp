// Fill out your copyright notice in the Description page of Project Settings.


#include "RAction.h"

#include "RActionComponent.h"

URAction::URAction() {
	
}

void URAction::StartAction_Implementation(AActor* Instigator) {
	UE_LOG(LogTemp, Warning, TEXT("Running %s"), *GetNameSafe(this))

	URActionComponent* ActionComponent = GetOwningComponent();
	ActionComponent->ActiveGameplayTags.AppendTags(GrantedTags);

	bIsRunning = true;
}

void URAction::StopAction_Implementation(AActor* Instigator) {
	UE_LOG(LogTemp, Warning, TEXT("Stopping %s"), *GetNameSafe(this))

	ensureAlways(bIsRunning);

	URActionComponent* ActionComponent = GetOwningComponent();
	ActionComponent->ActiveGameplayTags.RemoveTags(GrantedTags);

	bIsRunning = false;
}

bool URAction::CanStart_Implementation(AActor* Instigator) {
	if (IsRunning()) {
		return false;
	}
	URActionComponent* ActionComponent = GetOwningComponent();

	if (ActionComponent->ActiveGameplayTags.HasAny(BlockedTags)) {
		return false;
	}

	return true;
}

bool URAction::IsRunning() const {
	return bIsRunning;
}

UWorld* URAction::GetWorld() const {
	// Outer has been set through the new object
	UActorComponent* Comp = Cast<UActorComponent>(GetOuter());
	if (Comp) {
		return Comp->GetWorld();
	}
	return nullptr;
}

URActionComponent* URAction::GetOwningComponent() const {
	return Cast<URActionComponent>(GetOuter());
}
