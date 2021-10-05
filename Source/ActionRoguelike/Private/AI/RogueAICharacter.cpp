// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/RogueAICharacter.h"

#include "AIController.h"
#include "BrainComponent.h"
#include "DrawDebugHelpers.h"
#include "RogueAttributeComponent.h"
#include "AI/RogueAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/PawnSensingComponent.h"

// Sets default values
ARogueAICharacter::ARogueAICharacter()
{
	PawnSensingComponent = CreateDefaultSubobject<UPawnSensingComponent>("PawnSensingComponent");

	AttributeComponent = CreateDefaultSubobject<URogueAttributeComponent>("AttributeComponent");

	TimeToHitParamName = "TimeToHit";
	
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

void ARogueAICharacter::PostInitializeComponents() {
	Super::PostInitializeComponents();
	
	PawnSensingComponent->OnSeePawn.AddDynamic(this, &ARogueAICharacter::OnPawnSeen);
	AttributeComponent->OnHealthChanged.AddDynamic(this, &ARogueAICharacter::OnHealthChanged);
}

void ARogueAICharacter::OnPawnSeen(APawn* Pawn) {
	SetTargetActor(Pawn);
	DrawDebugString(GetWorld(), GetActorLocation(), "PLAYER SPOTTED", nullptr, FColor::White, 4.f, true);
}

void ARogueAICharacter::OnHealthChanged(AActor* InstigatorActor, URogueAttributeComponent* OwningComponent,
	float NewHealth, float Delta) {
	if (Delta < 0.f) {

		if (InstigatorActor != this) {
			SetTargetActor(InstigatorActor);
		}

		// Hit flash
		GetMesh()->SetScalarParameterValueOnMaterials(TimeToHitParamName, GetWorld()->GetTimeSeconds());
		
		if (NewHealth <= 0.f) {
			// Just died
			// Stop behavior tree, ragdoll, set lifespan(time before destruction)
			AAIController* AIController = Cast<ARogueAIController>(GetController());
			if (ensure(AIController)) {
				AIController->GetBrainComponent()->StopLogic("Killed");
			}
			GetMesh()->SetAllBodiesSimulatePhysics(true);
			GetMesh()->SetCollisionProfileName("Ragdoll");
			
			SetLifeSpan(10.f);
		}
	}
}

void ARogueAICharacter::SetTargetActor(AActor* NewTarget) {
	AAIController* AIController = Cast<AAIController>(GetController());
	if (AIController) {
		UBlackboardComponent* BlackboardComponent = AIController->GetBlackboardComponent();

		BlackboardComponent->SetValueAsObject("TargetActor", NewTarget);
	}
}