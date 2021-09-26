// Fill out your copyright notice in the Description page of Project Settings.

#include "RogueCharacter.h"

#include "RogueMagicProjectile.h"
#include "RogueInteractionComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ARogueCharacter::ARogueCharacter() {
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Spring arm
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArmComponent->SetupAttachment(RootComponent);

	// Camera
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComponent->SetupAttachment(SpringArmComponent);

	// Interaction
	InteractionComponent = CreateDefaultSubobject<URogueInteractionComponent>(TEXT("InteractionComponent"));

	GetCharacterMovement()->bOrientRotationToMovement = true; // This will rotate our character no matter what we're moving towards.
	GetCharacterMovement()->JumpZVelocity = 500.f;
	bUseControllerRotationYaw = false;
}

// Called when the game starts or when spawned
void ARogueCharacter::BeginPlay() { Super::BeginPlay(); }

// Called every frame
void ARogueCharacter::Tick(float DeltaTime) { Super::Tick(DeltaTime); }

// Called to bind functionality to input
void ARogueCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) {
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ARogueCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ARogueCharacter::MoveRight);
	
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);

	PlayerInputComponent->BindAction("PrimaryAttack", IE_Pressed, this, &ARogueCharacter::PrimaryAttack);
	PlayerInputComponent->BindAction("PrimaryInteract", IE_Pressed, this, &ARogueCharacter::PrimaryInteract);
}

void ARogueCharacter::MoveForward(float Value) {
	FRotator controlRot = GetControlRotation();
	controlRot.Pitch = 0.f;
	controlRot.Roll = 0.f;
	
	AddMovementInput(controlRot.Vector(), Value);
}

void ARogueCharacter::MoveRight(float Value) {
	FRotator controlRot = GetControlRotation();
	controlRot.Pitch = 0.f;
	controlRot.Roll = 0.f;

	FVector rightVector = FRotationMatrix(controlRot).GetScaledAxis(EAxis::Y);
	
	AddMovementInput(rightVector, Value);
}

void ARogueCharacter::PrimaryAttack() {
	PlayAnimMontage(AttackAnimation);
	GetWorldTimerManager().SetTimer(TimerHandle_PrimaryAttack, this, &ARogueCharacter::PrimaryAttack_TimeElapsed, .18f);
	// GetWorldTimerManager().ClearTimer(TimerHandle_PrimaryAttack);
}

void ARogueCharacter::PrimaryAttack_TimeElapsed() const {
	FTransform SpawnTM = FTransform(GetControlRotation(), GetMesh()->GetSocketLocation("Muzzle_01"));

	FActorSpawnParameters spawnParams;
	spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	
	GetWorld()->SpawnActor<AActor>(ProjectileClass, SpawnTM, spawnParams);
}


void ARogueCharacter::PrimaryInteract() {
	InteractionComponent->PrimaryInteract();
}
