// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/RogueBTService_CheckAttackRange.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

void URogueBTService_CheckAttackRange::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) {
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	// Check distance b/w AI pawn and target actor
	UBlackboardComponent* BlackboardComponent = OwnerComp.GetBlackboardComponent();
	if (ensure(BlackboardComponent)) {
		AActor* TargetActor = Cast<AActor>(BlackboardComponent->GetValueAsObject("TargetActor"));
		if (TargetActor) {
			AAIController* AIController = OwnerComp.GetAIOwner();
			if (ensure(AIController)) {
				APawn* AIPawn = AIController->GetPawn();
				if (ensure(AIPawn)) {
					float DistanceTo = FVector::Distance(TargetActor->GetActorLocation(), AIPawn->GetActorLocation());

					bool bIsWithinRange = DistanceTo < 2000.f;

					bool HasLineOfSightToTarget = false;
					if (bIsWithinRange) {
						HasLineOfSightToTarget = AIController->LineOfSightTo(TargetActor);
					}
					
					BlackboardComponent->SetValueAsBool(AttackRangeKey.SelectedKeyName, bIsWithinRange && HasLineOfSightToTarget);
				}
			}
		}
	}
}
