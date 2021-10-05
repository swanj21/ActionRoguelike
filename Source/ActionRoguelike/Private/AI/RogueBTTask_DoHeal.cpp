// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/RogueBTTask_DoHeal.h"

#include "AIController.h"
#include "RogueAttributeComponent.h"

URogueBTTask_DoHeal::URogueBTTask_DoHeal() {
	
}

EBTNodeResult::Type URogueBTTask_DoHeal::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) {
	Super::ExecuteTask(OwnerComp, NodeMemory);

	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController) {
		return EBTNodeResult::Failed;
	}

	APawn* Pawn = AIController->GetPawn();
	if (!Pawn) {
		return EBTNodeResult::Failed;
	}
	
	URogueAttributeComponent* AttributeComponent = URogueAttributeComponent::GetAttributes(Pawn);
	if (!AttributeComponent) {
		return EBTNodeResult::Failed;
	}

	// Heal up to max health
	bool bHealSuccessful = AttributeComponent->ApplyHealthChange(Pawn, AttributeComponent->GetMaxHealth());

	return bHealSuccessful ? EBTNodeResult::Succeeded : EBTNodeResult::Failed;
}
