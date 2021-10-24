// Fill out your copyright notice in the Description page of Project Settings.


#include "RogueItemChest.h"

#include "Net/UnrealNetwork.h"

// Sets default values
ARogueItemChest::ARogueItemChest()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>("BaseMesh");
	RootComponent = BaseMesh;

	LidMesh = CreateDefaultSubobject<UStaticMeshComponent>("LidMesh");
	LidMesh->SetupAttachment(BaseMesh);

	TargetPitch = 100;

	bReplicates = true;
}

void ARogueItemChest::OnActorLoaded_Implementation() {
	OnRep_LidOpened();
}

void ARogueItemChest::Interact_Implementation(APawn* InstigatorPawn) {
	bIsLidOpened = !bIsLidOpened;
	// Needs to be manually called on the server, automatically called for the client
	OnRep_LidOpened();
}

void ARogueItemChest::OnRep_LidOpened() {
	float CurPitch = bIsLidOpened ? TargetPitch : 0.f;
	LidMesh->SetRelativeRotation(FRotator(CurPitch, 0.f, 0.f));
}

void ARogueItemChest::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ARogueItemChest, bIsLidOpened);
}