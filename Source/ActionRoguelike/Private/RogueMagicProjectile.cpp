// Fill out your copyright notice in the Description page of Project Settings.


#include "RogueMagicProjectile.h"

#include "RActionComponent.h"
#include "RGameplayFunctionLibrary.h"
#include "Components/AudioComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "RActionEffect.h"

// Sets default values
ARogueMagicProjectile::ARogueMagicProjectile() {
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ARogueMagicProjectile::OnActorOverlap);

	Damage = 20.f;
}

// Called when the game starts or when spawned
void ARogueMagicProjectile::BeginPlay() {
	Super::BeginPlay();

	FlightAudioComponent->Play(.2f);
}

// Called every frame
void ARogueMagicProjectile::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}

void ARogueMagicProjectile::OnActorOverlap(UPrimitiveComponent* OverlappedComponent,
                                           AActor* OtherActor,
                                           UPrimitiveComponent* OtherComp,
                                           int32 OtherBodyIndex,
                                           bool bFromSweep,
                                           const FHitResult& SweepResult) {

	if (HitActor) {
		return;
	} // TODO: This feels like a short-term workaround

	if (OtherActor && OtherActor != GetInstigator()) {
		URActionComponent* ActionComponent = Cast<URActionComponent>(
			OtherActor->GetComponentByClass(URActionComponent::StaticClass()));
		if (ActionComponent && ActionComponent->ActiveGameplayTags.HasTag(ParryTag)) {
			MovementComponent->Velocity = -MovementComponent->Velocity;
			SetInstigator(Cast<APawn>(OtherActor));

			return;
		}
		HitActor = OtherActor;
		if (!IsPendingKill() &&
			URGameplayFunctionLibrary::ApplyDirectionalDamage(GetInstigator(), OtherActor, Damage, SweepResult)) {
			if (ActionComponent) {
				ActionComponent->AddAction(GetInstigator(), BurningActionClass);
			}
			Destroy();
		}
	}
}
