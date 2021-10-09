// Fill out your copyright notice in the Description page of Project Settings.


#include "RAction.h"

URAction::URAction() {
	
}

void URAction::StartAction_Implementation(AActor* Instigator) {
	UE_LOG(LogTemp, Warning, TEXT("Running %s"), *GetNameSafe(this))
}

void URAction::StopAction_Implementation(AActor* Instigator) {
	UE_LOG(LogTemp, Warning, TEXT("Stopping %s"), *GetNameSafe(this))
}

UWorld* URAction::GetWorld() const {
	UActorComponent* Comp = Cast<UActorComponent>(GetOuter());
	if (Comp) {
		return Comp->GetWorld();
	}
	return nullptr;
}