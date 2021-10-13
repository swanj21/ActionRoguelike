// Fill out your copyright notice in the Description page of Project Settings.


#include "RActionEffect.h"

#include "RActionComponent.h"

URActionEffect::URActionEffect() {
	bAutoStart = true;
}

void URActionEffect::ExecutePeriodicEffect_Implementation(AActor* Instigator) {
	UE_LOG(LogTemp, Warning, TEXT("Executing periodic effect"))
}

void URActionEffect::StartAction_Implementation(AActor* Instigator) {
	Super::StartAction_Implementation(Instigator);

	// Duration - How long should this effect last overall? (Enemy is on fire for 10 seconds)
	if (Duration > 0.f) {
		FTimerDelegate Delegate;
		Delegate.BindUFunction(this, "StopAction", Instigator);

		GetWorld()->GetTimerManager().SetTimer(DurationHandle, Delegate, Duration, false);
	}

	// Period - In what intervals should this effect happen? (Take burn damage every 2 seconds)
	if (Period > 0.f) {
		FTimerDelegate Delegate;
		Delegate.BindUFunction(this, "ExecutePeriodicEffect", Instigator);

		// We want the period to loop
		GetWorld()->GetTimerManager().SetTimer(PeriodHandle, Delegate, Period, true);
	}
}

void URActionEffect::StopAction_Implementation(AActor* Instigator) {
	if (GetWorld()->GetTimerManager().GetTimerRemaining(PeriodHandle) < KINDA_SMALL_NUMBER) {
		ExecutePeriodicEffect(Instigator);
	}
	
	Super::StopAction_Implementation(Instigator);

	GetWorld()->GetTimerManager().ClearTimer(DurationHandle);
	GetWorld()->GetTimerManager().ClearTimer(PeriodHandle);

	URActionComponent* ActionComponent = GetOwningComponent();

	if (ActionComponent) {
		ActionComponent->RemoveAction(this);
	}
}
