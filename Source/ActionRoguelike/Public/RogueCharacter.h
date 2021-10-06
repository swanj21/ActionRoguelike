// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RogueAttributeComponent.h"
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

	UPROPERTY(VisibleAnywhere, Category="Components")
	class UCameraComponent* CameraComponent;

	UPROPERTY(VisibleAnywhere, Category="Components")
	class USpringArmComponent* SpringArmComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	class URogueAttributeComponent* AttributeComponent;

	UPROPERTY(EditAnywhere, Category="Attack")
	TSubclassOf<AActor> PrimaryProjectileClass;

	UPROPERTY(EditAnywhere, Category="Attack")
	TSubclassOf<AActor> SecondaryProjectileClass;
	
	UPROPERTY(EditAnywhere, Category="Attack")
	class UAnimMontage* AttackAnimation;

	UPROPERTY(EditAnywhere, Category="Effects")
	class UParticleSystem* MuzzleFlash;

	UPROPERTY(VisibleAnywhere, Category="Effects")
	FName HandSocketName;

	UPROPERTY(VisibleAnywhere, Category="Effects")
	FName TimeToHitParamName;
	
	UPROPERTY(EditAnywhere, Category="Attack")
	float AttackDistance = 5000.f;

	UPROPERTY(EditAnywhere, Category="Teleport")
	TSubclassOf<AActor> TeleportProjectileClass;

	UPROPERTY(VisibleAnywhere, Category="Interaction")
	class URogueInteractionComponent* InteractionComponent;

	UFUNCTION(BlueprintCallable, Category="Health")
	void OnHealthChanged(AActor* InstigatorActor, class URogueAttributeComponent* OwningComponent, float NewHealth, float Delta);

	FTimerHandle TimerHandle_PrimaryAttack;
	FTimerHandle TimerHandle_SecondaryAttack;
	FTimerHandle TimerHandle_ProjectileTeleport;

	void MoveForward(float Value);
	void MoveRight(float Value);
	
	void PrimaryAttack();
	void ProjectileAttack(FString AttackType);
	void PrimaryAttack_TimeElapsed();
	void SecondaryAttack();
	void SecondaryAttack_TimeElapsed();
	void PrimaryInteract();
	void ProjectileTeleport();
	void ProjectileTeleport_TimeElapsed();
	void DoSpawnProjectile(TSubclassOf<AActor> ProjectileType);

	UFUNCTION(BlueprintCallable, Category="Aiming")
	FRotator FindAimRotation();

	virtual void PostInitializeComponents() override;
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(Exec)
	void HealSelf(float Amount = 100);
};
