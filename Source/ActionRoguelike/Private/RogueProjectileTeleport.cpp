// Fill out your copyright notice in the Description page of Project Settings.


#include "RogueProjectileTeleport.h"

#include "RogueCharacter.h"
#include "TimerManager.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"

ARogueProjectileTeleport::ARogueProjectileTeleport() {
	DestructionEffectComponent = CreateDefaultSubobject<UParticleSystemComponent>("DestructionEffectComponent");
	DestructionEffectComponent->SetupAttachment(SphereComponent);
}

void ARogueProjectileTeleport::BeginPlay() {
	Super::BeginPlay();
	
	GetWorldTimerManager().SetTimer(TimerHandle_Explode, this, &ARogueProjectileTeleport::Explode_TimeElapsed, .2f);
}

void ARogueProjectileTeleport::Tick(float DeltaSeconds) {
	Super::Tick(DeltaSeconds);
}

void ARogueProjectileTeleport::PostInitializeComponents() {
	Super::PostInitializeComponents();
	SphereComponent->OnComponentHit.AddDynamic(this, &ARogueProjectileTeleport::OnComponentHit);
}

void ARogueProjectileTeleport::Explode_TimeElapsed() {
	DoProjectileExplosion();
}

void ARogueProjectileTeleport::DoProjectileExplosion() {
	MovementComponent->StopMovementImmediately();
	DestructionEffectComponent->Activate();
	SetActorEnableCollision(false);
	
	GetWorldTimerManager().SetTimer(TimerHandle_Teleport, this, &ARogueProjectileTeleport::Teleport_TimeElapsed, .2f);
}

void ARogueProjectileTeleport::Teleport_TimeElapsed() {
	FVector Location = GetActorLocation();
	DoTeleportPlayer(Location);
	Destroy();
}

void ARogueProjectileTeleport::DoTeleportPlayer(FVector Location) {
	if (GetInstigator()->TeleportTo(Location, GetInstigator()->GetActorRotation())) {
		UE_LOG(LogTemp, Warning, TEXT("Teleported player"))
	} else {
		UE_LOG(LogTemp, Warning, TEXT("Could not teleport player"))
	}
}

// ReSharper disable once CppMemberFunctionMayBeConst
void ARogueProjectileTeleport::OnComponentHit(
	UPrimitiveComponent* HitComp,
	// ReSharper disable once CppParameterMayBeConstPtrOrRef
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	FVector NormalImpulse,
	const FHitResult& Hit) {
	if (OtherActor != GetInstigator()) {
		GetWorldTimerManager().ClearTimer(TimerHandle_Explode);
		DoProjectileExplosion();
	}
}