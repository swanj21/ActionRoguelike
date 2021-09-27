// Fill out your copyright notice in the Description page of Project Settings.


#include "RogueProjectile.h"

#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
ARogueProjectile::ARogueProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereComponent = CreateDefaultSubobject<USphereComponent>("SphereComponent");
	SphereComponent->SetCollisionObjectType(ECC_WorldDynamic);
	SphereComponent->SetCollisionProfileName("Projectile");
	RootComponent = SphereComponent;

	EffectComponent = CreateDefaultSubobject<UParticleSystemComponent>("EffectComponent");
	EffectComponent->SetupAttachment(SphereComponent);
	
	MovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>("MovementComponent");
	MovementComponent->InitialSpeed = 3000.f;
	MovementComponent->bRotationFollowsVelocity = true;
	MovementComponent->bInitialVelocityInLocalSpace = true;
}

// Called when the game starts or when spawned
void ARogueProjectile::BeginPlay()
{
	Super::BeginPlay();

	SphereComponent->IgnoreActorWhenMoving(GetInstigator(), true);
}

// Called every frame
void ARogueProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

