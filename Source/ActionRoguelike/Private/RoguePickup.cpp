// Fill out your copyright notice in the Description page of Project Settings.


#include "RoguePickup.h"

// Sets default values
ARoguePickup::ARoguePickup()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Mesh, Collider
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("Static Mesh");
	RootComponent = StaticMeshComponent;

}

// Called when the game starts or when spawned
void ARoguePickup::BeginPlay()
{
	Super::BeginPlay();
	
}

void ARoguePickup::DisableItem() {
	// Could also do RootComponent->SetVisibility(false, true) where the 'true' tells it to propagate to all children.
	StaticMeshComponent->SetVisibility(false);
	IsActive = false;
}

void ARoguePickup::EnableItem() {
	StaticMeshComponent->SetVisibility(true);
	IsActive = true;
}

// Called every frame
void ARoguePickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ARoguePickup::Interact_Implementation(APawn* InstigatorPawn) {
	UE_LOG(LogTemp, Warning, TEXT("Interact called on base RoguePickup class, should be overridden"))
}
