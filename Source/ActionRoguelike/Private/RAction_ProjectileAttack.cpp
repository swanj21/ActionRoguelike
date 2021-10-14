// Fill out your copyright notice in the Description page of Project Settings.


#include "RAction_ProjectileAttack.h"

#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

URAction_ProjectileAttack::URAction_ProjectileAttack() {
	HandSocketName = "Muzzle_01";
	AttackAnimDelay = .2f;
}

void URAction_ProjectileAttack::StartAction_Implementation(AActor* Instigator) {
	Super::StartAction_Implementation(Instigator);

	ACharacter* Character = Cast<ACharacter>(Instigator);
	if (Character) {
		StartAnim(Character);
		SetTimer(Character);
	}
}

void URAction_ProjectileAttack::AttackDelay_Elapsed(ACharacter* InstigatorCharacter) {
	if (ensureAlways(ProjectileClass)) {
		FVector HandLocation = InstigatorCharacter->GetMesh()->GetSocketLocation(HandSocketName);
		FActorSpawnParameters SpawnParameters;
		SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParameters.Instigator = InstigatorCharacter;

		FCollisionShape Shape;
		Shape.SetSphere(20.f);

		// Ignore player
		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(InstigatorCharacter);

		FCollisionObjectQueryParams ObjectParams;
		ObjectParams.AddObjectTypesToQuery(ECC_WorldDynamic);
		ObjectParams.AddObjectTypesToQuery(ECC_WorldStatic);
		ObjectParams.AddObjectTypesToQuery(ECC_Pawn);

		FVector TraceStart = InstigatorCharacter->GetPawnViewLocation();

		FVector TraceEnd = TraceStart + (InstigatorCharacter->GetControlRotation().Vector() * 5000);

		FHitResult Hit;

		if (GetWorld()->SweepSingleByObjectType(Hit, TraceStart, TraceEnd, FQuat::Identity, ObjectParams, Shape, QueryParams)) {
			TraceEnd = Hit.ImpactPoint;
		}

		FRotator ProjectileRotation = FRotationMatrix::MakeFromX(TraceEnd - HandLocation).Rotator();

		FTransform SpawnTransform = FTransform(ProjectileRotation, HandLocation);
		GetWorld()->SpawnActor<AActor>(ProjectileClass, SpawnTransform, SpawnParameters);
	}

	StopAction(InstigatorCharacter);
}

void URAction_ProjectileAttack::StartAnim(ACharacter* Character) const {
	if (!ensure(Character)) {
		return;
	}
	Character->PlayAnimMontage(AttackAnimation);

	UGameplayStatics::SpawnEmitterAttached(MuzzleFlash, Character->GetMesh(), HandSocketName, FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::SnapToTarget);
}

void URAction_ProjectileAttack::SetTimer(ACharacter* Character) {
	if (!ensure(Character)) {
		return;
	}

	FTimerDelegate Delegate;
	Delegate.BindUFunction(this, "AttackDelay_Elapsed", Character);

	GetWorld()->GetTimerManager().SetTimer(TimerHandle_AttackDelay, Delegate, AttackAnimDelay, false);
}