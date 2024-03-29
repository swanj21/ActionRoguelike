// Fill out your copyright notice in the Description page of Project Settings.


#include "RoguePickup.h"

#include "ActionRoguelike/ActionRoguelike.h"
#include "Net/UnrealNetwork.h"

// Sets default values
ARoguePickup::ARoguePickup()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Mesh, Collider
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("Static Mesh");
	RootComponent = StaticMeshComponent;

	RespawnTimer = 10.f;

	bReplicates = true;
}

// Called when the game starts or when spawned
void ARoguePickup::BeginPlay()
{
	Super::BeginPlay();
	
}

void ARoguePickup::DisableItem() {
	// Could also do RootComponent->SetVisibility(false, true) where the 'true' tells it to propagate to all children.
	if (HasAuthority()) {
		bIsActive = false;
		ChangeVisibility(false);
	}
}

void ARoguePickup::EnableItem() {
	if (HasAuthority()) {
		bIsActive = true;
		ChangeVisibility(true);
	}
}

void ARoguePickup::ChangeVisibility(bool bIsVisible) {
	StaticMeshComponent->SetVisibility(bIsVisible);
	SetActorEnableCollision(bIsVisible);
	LogOnScreen(this, FString::Printf(TEXT("%s visibility changed to %hhd"), *GetNameSafe(this), bIsActive));
}

void ARoguePickup::OnRep_ActiveChanged() {
	ChangeVisibility(bIsActive);
}

void ARoguePickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ARoguePickup::Interact_Implementation(APawn* InstigatorPawn) {
	UE_LOG(LogTemp, Warning, TEXT("Interact called on base RoguePickup class, should be overridden"))
}

FText ARoguePickup::GetInteractText_Implementation(APawn* InstigatorPawn) {
	return FText::GetEmpty();
}

void ARoguePickup::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(ARoguePickup, bIsActive);
}
