// Fill out your copyright notice in the Description page of Project Settings.


#include "RogueTargetDummy.h"
#include "RogueAttributeComponent.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
ARogueTargetDummy::ARogueTargetDummy()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Static mesh
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComponent");
	RootComponent = StaticMeshComponent;
	
	// Attributes
	AttributeComponent = CreateDefaultSubobject<URogueAttributeComponent>("AttributeComponent");
}

// Called when the game starts or when spawned
void ARogueTargetDummy::BeginPlay()
{
	Super::BeginPlay();

	AttributeComponent->OnHealthChanged.AddDynamic(this, &ARogueTargetDummy::OnHealthChanged);
}

// Called every frame
void ARogueTargetDummy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ARogueTargetDummy::OnHealthChanged(AActor* InstigatorActor, class URogueAttributeComponent* OwningComponent, float NewHealth, float Delta) {
	if (Delta < 0) { // Took damage
		StaticMeshComponent->SetScalarParameterValueOnMaterials("TimeToHit", GetWorld()->GetTimeSeconds());
	}
}