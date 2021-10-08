// Fill out your copyright notice in the Description page of Project Settings.


#include "RogueMagicProjectile.h"

#include "RGameplayFunctionLibrary.h"
#include "Components/AudioComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ARogueMagicProjectile::ARogueMagicProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ARogueMagicProjectile::OnActorOverlap);

	Damage = 20.f;
}

// Called when the game starts or when spawned
void ARogueMagicProjectile::BeginPlay()
{
	Super::BeginPlay();

	FlightAudioComponent->Play(.2f);
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

	if (HitActor) { return; } // TODO: This feels like a short-term workaround

	UE_LOG(LogTemp, Warning, TEXT("Overlapping %s"), *GetNameSafe(OtherComp))

	if (OtherActor && OtherActor != GetInstigator()) {
		HitActor = OtherActor;
		UE_LOG(LogTemp, Warning, TEXT("Attempting to damage %s"), *GetNameSafe(OtherActor))
		if (!IsPendingKill() && URGameplayFunctionLibrary::ApplyDirectionalDamage(GetInstigator(), OtherActor, Damage, SweepResult)) {
			UE_LOG(LogTemp, Warning, TEXT("Successfully damaged %s, now being destroyed"), *GetNameSafe(OtherActor))
			Destroy();
		}
	}
}


