// Fill out your copyright notice in the Description page of Project Settings.


#include "RActionComponent.h"

#include "RAction.h"
#include "ActionRoguelike/ActionRoguelike.h"
#include "Engine/ActorChannel.h"
#include "Net/UnrealNetwork.h"

// Sets default values for this component's properties
URActionComponent::URActionComponent() {
	PrimaryComponentTick.bCanEverTick = true;

	SetIsReplicatedByDefault(true);
}

void URActionComponent::BeginPlay() {
	Super::BeginPlay();

	// Server only
	if (GetOwner()->HasAuthority()) {
		for (TSubclassOf<URAction> ActionClass : DefaultActions) {
			AddAction(GetOwner(), ActionClass);
		}
	}
}

void URActionComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                      FActorComponentTickFunction* ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Draw all actions
	for (URAction* Action : Actions) {
		FColor TextColor = Action->IsRunning() ? FColor::Blue : FColor::White;

		FString ActionMessage = FString::Printf(TEXT("[%s] Action: %s"),
			*GetNameSafe(GetOwner()),
			*Action->ActionName.ToString());

		LogOnScreen(this, ActionMessage, TextColor, 0.f);
	}
}

bool URActionComponent::AddAction(AActor* Instigator, TSubclassOf<URAction> ActionClass) {
	if (!ensure(ActionClass)) {
		return false;
	}

	if (!GetOwner()->HasAuthority()) {
		UE_LOG(LogTemp, Warning, TEXT("Client attempting to add action.  Class %s"), *GetNameSafe(ActionClass))
		return false;
	}
	
	URAction* NewAction = NewObject<URAction>(GetOwner(), ActionClass);
	if (!ensure(NewAction)) {
		return false;
	}

	NewAction->Initialize(this);

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
	if (ensure(ActionToRemove) && !ActionToRemove->IsRunning()) {
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

			// Is client
			if (!GetOwner()->HasAuthority()) {
				ServerStartAction(Instigator, ActionName);
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
				if (!GetOwner()->HasAuthority()) {
					ServerStopAction(Instigator, ActionName);
				}
				Action->StopAction(Instigator);
				return true;
			}
		}
	}
	return false;
}

void URActionComponent::ServerStartAction_Implementation(AActor* Instigator, FName ActionName) {
	StartActionByName(Instigator, ActionName);
}

void URActionComponent::ServerStopAction_Implementation(AActor* Instigator, FName ActionName) {
	StopActionByName(Instigator, ActionName);
}

bool URActionComponent::ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags) {
	bool WroteSomething = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);
	for (URAction* Action : Actions) {
		if (Action) {
			// Bitwise OR
			WroteSomething |= Channel->ReplicateSubobject(Action, *Bunch, *RepFlags);
		}
	}

	return WroteSomething;
}

void URActionComponent::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(URActionComponent, Actions);
}