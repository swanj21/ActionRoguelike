// Fill out your copyright notice in the Description page of Project Settings.


#include "RogueExplosiveBarrel.h"

#include "DrawDebugHelpers.h"
#include "RogueAttributeComponent.h"
#include "PhysicsEngine/RadialForceComponent.h"

// Sets default values
ARogueExplosiveBarrel::ARogueExplosiveBarrel() {
	PrimaryActorTick.bCanEverTick = true;

	// Setting up the Static Mesh component
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("Static Mesh");
	StaticMesh->SetGenerateOverlapEvents(true);
	StaticMesh->SetSimulatePhysics(true);
	StaticMesh->SetGenerateOverlapEvents(true);
	RootComponent = StaticMesh;

	// Setting up the Radial Force component
	RadialForce = CreateDefaultSubobject<URadialForceComponent>("Radial Force");
	RadialForce->SetupAttachment(StaticMesh);
	RadialForce->Radius = 500.f;
	RadialForce->ImpulseStrength = 1000.f;
	RadialForce->bImpulseVelChange = true;

}

// Called when the game starts or when spawned
void ARogueExplosiveBarrel::BeginPlay() {
	Super::BeginPlay();
}

// Called every frame
void ARogueExplosiveBarrel::Tick(float DeltaTime) { Super::Tick(DeltaTime); }

void ARogueExplosiveBarrel::PostInitializeComponents() {
	Super::PostInitializeComponents();
	StaticMesh->OnComponentHit.AddDynamic(this, &ARogueExplosiveBarrel::OnComponentHit);
}

// ReSharper disable once CppMemberFunctionMayBeConst
void ARogueExplosiveBarrel::OnComponentHit(
	UPrimitiveComponent* HitComp,
	// ReSharper disable once CppParameterMayBeConstPtrOrRef
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	FVector NormalImpulse,
	const FHitResult& Hit) {
	if (RadialForce && OtherActor->ActorHasTag("Projectile")) {
		UE_LOG(LogTemp, Warning, TEXT("Barrel hit by projectile"))
		RadialForce->FireImpulse();
	}
}
