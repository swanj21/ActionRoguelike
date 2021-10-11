// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/RogueAICharacter.h"

#include "AIController.h"
#include "BrainComponent.h"
#include "DrawDebugHelpers.h"
#include "RActionComponent.h"
#include "RGameplayFunctionLibrary.h"
#include "RogueAttributeComponent.h"
#include "RWorldUserWidget.h"
#include "AI/RogueAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Blueprint/UserWidget.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Perception/PawnSensingComponent.h"

// Sets default values
ARogueAICharacter::ARogueAICharacter()
{
	PawnSensingComponent = CreateDefaultSubobject<UPawnSensingComponent>("PawnSensingComponent");

	AttributeComponent = CreateDefaultSubobject<URogueAttributeComponent>("AttributeComponent");

	ActionComponent = CreateDefaultSubobject<URActionComponent>("ActionComponent");

	TimeToHitParamName = "TimeToHit";

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Ignore);
	GetMesh()->SetGenerateOverlapEvents(true);

	CreditsToGive = 10.f;
	
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

		if (!ActiveHealthBar) {
			ActiveHealthBar = CreateWidget<URWorldUserWidget>(GetWorld(), HealthBarWidgetClass);
			if (ActiveHealthBar) {
				ActiveHealthBar->AttachedActor = this;
				ActiveHealthBar->AddToViewport();
			}
		}
		
		// Hit flash
		GetMesh()->SetScalarParameterValueOnMaterials(TimeToHitParamName, GetWorld()->GetTimeSeconds());

		// Just died
		if (NewHealth <= 0.f) {
			AAIController* AIController = Cast<ARogueAIController>(GetController());
			if (ensure(AIController)) {
				AIController->GetBrainComponent()->StopLogic("Killed");
			}
			
			GetMesh()->SetAllBodiesSimulatePhysics(true);
			GetMesh()->SetCollisionProfileName("Ragdoll");
			GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			GetCharacterMovement()->DisableMovement();

			UE_LOG(LogTemp, Warning, TEXT("Giving %f credits to %s because of kill"), CreditsToGive, *GetNameSafe(InstigatorActor))
			URGameplayFunctionLibrary::GiveCredits(InstigatorActor->GetInstigatorController(), CreditsToGive);

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
