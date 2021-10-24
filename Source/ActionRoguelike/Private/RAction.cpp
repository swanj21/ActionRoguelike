// Fill out your copyright notice in the Description page of Project Settings.


#include "RAction.h"

#include "RActionComponent.h"
#include "ActionRoguelike/ActionRoguelike.h"
#include "Net/UnrealNetwork.h"

URAction::URAction() {
	
}

void URAction::Initialize(URActionComponent* NewActionComponent) {
	ActionComp = NewActionComponent;
}

void URAction::StartAction_Implementation(AActor* Instigator) {
	//LogOnScreen(this, FString::Printf(TEXT("Started %s"), *ActionName.ToString()), FColor::Green);
	URActionComponent* NewActionComp = GetOwningComponent();
	NewActionComp->ActiveGameplayTags.AppendTags(GrantedTags);

	RepData.bIsRunning = true;
	RepData.Instigator = Instigator;
}

void URAction::StopAction_Implementation(AActor* Instigator) {
	//LogOnScreen(this, FString::Printf(TEXT("Stopped %s"), *ActionName.ToString()), FColor::Green);

	URActionComponent* NewActionComp = GetOwningComponent();
	NewActionComp->ActiveGameplayTags.RemoveTags(GrantedTags);

	RepData.bIsRunning = false;
	RepData.Instigator = Instigator;
}

bool URAction::CanStart_Implementation(AActor* Instigator) {
	if (IsRunning()) {
		return false;
	}
	URActionComponent* NewActionComp = GetOwningComponent();

	if (NewActionComp->ActiveGameplayTags.HasAny(BlockedTags)) {
		return false;
	}

	return true;
}

bool URAction::IsRunning() const {
	return RepData.bIsRunning;
}

UWorld* URAction::GetWorld() const {
	// Outer has been set through the new object
	AActor* Actor = Cast<AActor>(GetOuter());
	if (Actor) {
		return Actor->GetWorld();
	}
	return nullptr;
}

URActionComponent* URAction::GetOwningComponent() const {
	return ActionComp;
}

void URAction::OnRep_RepData() {
	// Server says "Hey I'm running, please start."
	if (RepData.bIsRunning) {
		StartAction(RepData.Instigator);
	} else {
		StopAction(RepData.Instigator);
	}
}

void URAction::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(URAction, ActionComp);
	DOREPLIFETIME(URAction, RepData);
}
