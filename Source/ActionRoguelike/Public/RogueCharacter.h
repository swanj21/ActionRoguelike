// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "RogueCharacter.generated.h"

UCLASS()
class ACTIONROGUELIKE_API ARogueCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ARogueCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category="Camera")
	class UCameraComponent* CameraComponent;

	UPROPERTY(VisibleAnywhere, Category="Camera")
	class USpringArmComponent* SpringArmComponent;

	UPROPERTY(EditAnywhere, Category="Attack")
	TSubclassOf<AActor> ProjectileClass;
	
	UPROPERTY(EditAnywhere, Category="Attack")
	class UAnimMontage* AttackAnimation;
	
	UPROPERTY(EditAnywhere, Category="Attack")
	float AttackDistance = 5000.f;

	UPROPERTY(VisibleAnywhere, Category="Interaction")
	class URogueInteractionComponent* InteractionComponent;

	FTimerHandle TimerHandle_PrimaryAttack;

	void MoveForward(float Value);
	void MoveRight(float Value);
	
	void PrimaryAttack();
	void PrimaryAttack_TimeElapsed();
	void PrimaryInteract();

	UFUNCTION(BlueprintCallable, Category="Aiming")
	FRotator FindAimRotation();
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
