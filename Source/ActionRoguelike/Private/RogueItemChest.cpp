// Fill out your copyright notice in the Description page of Project Settings.


#include "RogueItemChest.h"

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
}

// Called when the game starts or when spawned
void ARogueItemChest::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARogueItemChest::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ARogueItemChest::Interact_Implementation(APawn* InstigatorPawn) {
	LidMesh->SetRelativeRotation(FRotator(TargetPitch, 0.f, 0.f));
}