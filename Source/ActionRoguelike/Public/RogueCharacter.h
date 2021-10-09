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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	class URActionComponent* ActionComponent;

	UPROPERTY(EditAnywhere, Category="Attack")
	TSubclassOf<AActor> PrimaryProjectileClass;

	UPROPERTY(EditAnywhere, Category="Attack")
	TSubclassOf<AActor> SecondaryProjectileClass;

	UPROPERTY(VisibleAnywhere, Category="Effects")
	FName TimeToHitParamName;

	UPROPERTY(VisibleAnywhere, Category="Interaction")
	class URogueInteractionComponent* InteractionComponent;

	UFUNCTION(BlueprintCallable, Category="Health")
	void OnHealthChanged(AActor* InstigatorActor, class URogueAttributeComponent* OwningComponent, float NewHealth, float Delta);

	FTimerHandle TimerHandle_PrimaryAttack;
	FTimerHandle TimerHandle_SecondaryAttack;
	FTimerHandle TimerHandle_ProjectileTeleport;

	void MoveForward(float Value);
	void MoveRight(float Value);
	
	virtual FVector GetPawnViewLocation() const override;

	virtual void PostInitializeComponents() override;
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(Exec)
	void HealSelf(float Amount = 100);

	// --------- //
	// ABILITIES //
	// --------- //
	protected:

	UFUNCTION()
	void SprintStart();

	UFUNCTION()
	void SprintStop();

	void PrimaryAttack();
	void SecondaryAttack();
	void PrimaryInteract();
	void ProjectileTeleport();
};
