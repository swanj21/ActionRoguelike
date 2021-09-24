// Fill out your copyright notice in the Description page of Project Settings.

#include "RogueCharacter.h"

#include "RogueMagicProjectile.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ARogueCharacter::ARogueCharacter() {
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArmComponent->SetupAttachment(RootComponent);

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComponent->SetupAttachment(SpringArmComponent);

	GetCharacterMovement()->bOrientRotationToMovement = true; // This will rotate our character no matter what we're moving towards.
	
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

	PlayerInputComponent->BindAction("PrimaryAttack", IE_Pressed, this, &ARogueCharacter::PrimaryAttack);
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
	FTransform SpawnTM = FTransform(GetControlRotation(), GetMesh()->GetSocketLocation("Muzzle_01"));

	FActorSpawnParameters spawnParams;
	spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	
	GetWorld()->SpawnActor<AActor>(ProjectileClass, SpawnTM, spawnParams);
}
