// Fill out your copyright notice in the Description page of Project Settings.


#include "RogueMagicProjectile.h"

#include "RogueAttributeComponent.h"
#include "Components/SphereComponent.h"

// Sets default values
ARogueMagicProjectile::ARogueMagicProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ARogueMagicProjectile::OnActorOverlap);
}

// Called when the game starts or when spawned
void ARogueMagicProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARogueMagicProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ARogueMagicProjectile::OnActorOverlap(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult & SweepResult) {
	if (OtherActor && OtherActor != GetInstigator()) {
		URogueAttributeComponent* AttributeComponent = Cast<URogueAttributeComponent>(OtherActor->GetComponentByClass(URogueAttributeComponent::StaticClass()));
		if (AttributeComponent) {
			AttributeComponent->ApplyHealthChange(-1 * Damage);

			Destroy();
		}
	}
}


