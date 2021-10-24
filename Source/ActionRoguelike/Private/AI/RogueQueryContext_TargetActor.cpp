// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/RogueQueryContext_TargetActor.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "Kismet/GameplayStatics.h"

URogueQueryContext_TargetActor::URogueQueryContext_TargetActor() {
	TargetActorKey = "TargetActor";
}

AActor* URogueQueryContext_TargetActor::ProvideSingleActor(UObject* QuerierObject, AActor* QuerierActor, AActor*& ResultingActor) const {
	 if (ensure(QuerierActor)) {
	 	AAIController* AIController = UAIBlueprintHelperLibrary::GetAIController(QuerierActor);
	 	if (AIController) {
	 		UBlackboardComponent* Blackboard = AIController->GetBlackboardComponent();
	 		if (ensure(Blackboard)) {
	 			return Cast<AActor>(Blackboard->GetValueAsObject(TargetActorKey));
	 		}
	 	}
	 }
	// By default, give the position of the first player pawn(sucks for player 1)
	return UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
}

void URogueQueryContext_TargetActor::ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const {
	Super::ProvideContext(QueryInstance, ContextData);

	AActor* OutActor;
	ProvideSingleActor(nullptr, Cast<AActor>(QueryInstance.Owner), OutActor);
}
