// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/RogueAIController.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"

void ARogueAIController::BeginPlay() {
	Super::BeginPlay();

	if (ensureMsgf(BehaviorTree, TEXT("Behavior tree is null!  Please assign BehaviorTree in AI Controller"))) {
		RunBehaviorTree(BehaviorTree);
	}

	// DEPRECATED
	// APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(this, 0);
	//
	// if (ensure(PlayerPawn)) {
	// 	GetBlackboardComponent()->SetValueAsVector("MoveToLocation", PlayerPawn->GetActorLocation());
	//
	// 	GetBlackboardComponent()->SetValueAsObject("TargetActor", PlayerPawn);
	// }
}
