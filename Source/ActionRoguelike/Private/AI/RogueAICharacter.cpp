// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/RogueAICharacter.h"

#include "AIController.h"
#include "BrainComponent.h"
#include "DrawDebugHelpers.h"
#include "RActionComponent.h"
#include "RGameplayFunctionLibrary.h"
#include "RogueAttributeComponent.h"
#include "RWorldUserWidget.h"
#include "ActionRoguelike/ActionRoguelike.h"
#include "AI/RogueAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Blueprint/UserWidget.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Perception/PawnSensingComponent.h"

// Sets default values
ARogueAICharacter::ARogueAICharacter() {
	PawnSensingComponent = CreateDefaultSubobject<UPawnSensingComponent>("PawnSensingComponent");

	AttributeComponent = CreateDefaultSubobject<URogueAttributeComponent>("AttributeComponent");

	ActionComponent = CreateDefaultSubobject<URActionComponent>("ActionComponent");

	TimeToHitParamName = "TimeToHit";

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Ignore);
	GetMesh()->SetGenerateOverlapEvents(true);

	CreditsToGive = 10.f;
	SpottedDisplayTime = 2.f;

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

void ARogueAICharacter::PostInitializeComponents() {
	Super::PostInitializeComponents();

	PawnSensingComponent->OnSeePawn.AddDynamic(this, &ARogueAICharacter::OnPawnSeen);
	AttributeComponent->OnHealthChanged.AddDynamic(this, &ARogueAICharacter::OnHealthChanged);
}

void ARogueAICharacter::OnPawnSeen(APawn* Pawn) {
	SetTargetActor(Pawn);
}

void ARogueAICharacter::MulticastPawnSeen_Implementation() {
	if (!ActiveSpottedWidget) {
		ActiveSpottedWidget = CreateWidget<URWorldUserWidget>(GetWorld(), SpottedWidgetClass);
		if (ActiveSpottedWidget) {
			ActiveSpottedWidget->AttachedActor = this;
			ActiveSpottedWidget->AddToViewport(10);

			FTimerHandle TimerHandle;
			GetWorldTimerManager().SetTimer(TimerHandle, this, &ARogueAICharacter::OnSpottedTimerFinished,
			                                SpottedDisplayTime);
		}
	}
}

void ARogueAICharacter::OnHealthChanged(AActor* InstigatorActor, URogueAttributeComponent* OwningComponent,
                                        float NewHealth, float Delta) {
	if (Delta < 0.f) {

		if (InstigatorActor != this) {
			SetTargetActor(InstigatorActor);
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

			URGameplayFunctionLibrary::GiveCredits(InstigatorActor->GetInstigatorController(), CreditsToGive);

			SetLifeSpan(10.f);
		}
	}
}

void ARogueAICharacter::SetTargetActor(AActor* NewTarget) {
	UBlackboardComponent* BlackboardComponent = GetBlackboard();

	if (BlackboardComponent) {
		AActor* CurrentTarget = Cast<AActor>(BlackboardComponent->GetValueAsObject("TargetActor"));
		if (CurrentTarget != NewTarget) {
			BlackboardComponent->SetValueAsObject("TargetActor", NewTarget);
			// Create the player spotted widget and set a timer to destroy it
			MulticastPawnSeen();
		}
	}
}

UBlackboardComponent* ARogueAICharacter::GetBlackboard() {
	AAIController* AIController = Cast<AAIController>(GetController());
	if (AIController) {
		return AIController->GetBlackboardComponent();
	}
	return nullptr;
}

void ARogueAICharacter::OnSpottedTimerFinished() {
	LogOnScreen(this, FString::Printf(TEXT("Spotted timer finished")), FColor::Blue);
	if (ActiveSpottedWidget) {
		ActiveSpottedWidget->RemoveFromParent();
		ActiveSpottedWidget = nullptr;
	}
}