// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/RogueBTService_CheckCurHealth.h"

#include "AIController.h"
#include "RogueAttributeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

void URogueBTService_CheckCurHealth::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) {
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	// Find the current health of the AI character & save it in the blackboard key
	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
	if (ensure(Blackboard)) {
		AAIController* AIController = OwnerComp.GetAIOwner();
		if (AIController) {
			URogueAttributeComponent* AttributeComponent = URogueAttributeComponent::GetAttributes(AIController->GetPawn());
			if (AttributeComponent) {
				Blackboard->SetValueAsBool(CurrentHealthKey.SelectedKeyName, AttributeComponent->IsLowHealth());
			}
		}
	}
}
