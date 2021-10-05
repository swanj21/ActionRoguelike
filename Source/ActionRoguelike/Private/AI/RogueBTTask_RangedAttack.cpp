// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/RogueBTTask_RangedAttack.h"

#include "AIController.h"
#include "RogueAttributeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"

URogueBTTask_RangedAttack::URogueBTTask_RangedAttack() {
	MaxBulletSpread = 2.f;
}

EBTNodeResult::Type URogueBTTask_RangedAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) {
	AAIController* AIController = OwnerComp.GetAIOwner();

	if (ensure(AIController)) {
		ACharacter* AICharacter = Cast<ACharacter>(AIController->GetPawn());
		if (AICharacter == nullptr) {
			return EBTNodeResult::Failed;
		}

		FVector MuzzleLocation = AICharacter->GetMesh()->GetSocketLocation("Muzzle_01");

		AActor* TargetActor = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("TargetActor"));
		if (TargetActor == nullptr || !URogueAttributeComponent::IsActorAlive(TargetActor)) {
			return EBTNodeResult::Failed;
		}

		FVector Direction = TargetActor->GetActorLocation() - MuzzleLocation;
		FRotator MuzzleRotation = Direction.Rotation();

		MuzzleRotation.Pitch += FMath::RandRange(0.f, MaxBulletSpread);
		MuzzleRotation.Yaw += FMath::RandRange(-MaxBulletSpread, MaxBulletSpread);
		

		FActorSpawnParameters Params;
		Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		Params.Instigator = AICharacter;

		AActor* Projectile = GetWorld()->SpawnActor<AActor>(ProjectileClass, MuzzleLocation, MuzzleRotation, Params);
		
		return Projectile ? EBTNodeResult::Succeeded : EBTNodeResult::Failed;
	}
	return EBTNodeResult::Failed;
}
