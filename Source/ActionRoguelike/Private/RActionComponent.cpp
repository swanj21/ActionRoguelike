// Fill out your copyright notice in the Description page of Project Settings.


#include "RActionComponent.h"

#include "RAction.h"

// Sets default values for this component's properties
URActionComponent::URActionComponent() {
	PrimaryComponentTick.bCanEverTick = true;
}

void URActionComponent::BeginPlay() {
	Super::BeginPlay();

	for (TSubclassOf<URAction> ActionClass : DefaultActions) {
		AddAction(GetOwner(), ActionClass);
	}
}


void URActionComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                      FActorComponentTickFunction* ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

bool URActionComponent::AddAction(AActor* Instigator, TSubclassOf<URAction> ActionClass) {
	if (!ensure(ActionClass)) {
		return false;
	}
	URAction* NewAction = NewObject<URAction>(this, ActionClass);
	if (!ensure(NewAction)) {
		return false;
	}

	for (URAction* Action : Actions) {
		if (Action->ActionName.IsEqual(NewAction->ActionName)) {
			return false;
		}
	}

	Actions.Add(NewAction);

	if (NewAction->bAutoStart && ensure(NewAction->CanStart(Instigator))) {
		NewAction->StartAction(Instigator);
	}
	
	return true;
}

void URActionComponent::RemoveAction(URAction* ActionToRemove) {
	if (ensure(ActionToRemove && !ActionToRemove->IsRunning())) {
		return;
	}

	Actions.Remove(ActionToRemove);
}

bool URActionComponent::StartActionByName(AActor* Instigator, FName ActionName) {
	for (URAction* Action : Actions) {
		if (Action && Action->ActionName == ActionName) {
			if (!Action->CanStart(Instigator)) {
				continue;
			}
			Action->StartAction(Instigator);
			return true;
		}
	}
	return false;
}

bool URActionComponent::StopActionByName(AActor* Instigator, FName ActionName) {
	for (URAction* Action : Actions) {
		if (Action && Action->ActionName == ActionName) {
			if (Action->IsRunning()) {
				Action->StopAction(Instigator);
				return true;
			}
		}
	}
	return false;
}
