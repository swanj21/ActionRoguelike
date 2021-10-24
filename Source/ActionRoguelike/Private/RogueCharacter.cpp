// Fill out your copyright notice in the Description page of Project Settings.

#include "RogueCharacter.h"

#include "RActionComponent.h"
#include "RogueInteractionComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"

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

	// Attributes
	AttributeComponent = CreateDefaultSubobject<URogueAttributeComponent>(TEXT("AttributeComponent"));

	// Actions
	ActionComponent = CreateDefaultSubobject<URActionComponent>("ActionComponent");

	GetCharacterMovement()->bOrientRotationToMovement = true; // This will rotate our character no matter what we're moving towards.
	GetCharacterMovement()->JumpZVelocity = 500.f;
	bUseControllerRotationYaw = false;

	TimeToHitParamName = "TimeToHit";
}

void ARogueCharacter::PostInitializeComponents() {
	Super::PostInitializeComponents();
	AttributeComponent->OnHealthChanged.AddDynamic(this, &ARogueCharacter::OnHealthChanged);
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
	PlayerInputComponent->BindAction("SecondaryAttack", IE_Pressed, this, &ARogueCharacter::SecondaryAttack);
	PlayerInputComponent->BindAction("PrimaryInteract", IE_Pressed, this, &ARogueCharacter::PrimaryInteract);
	PlayerInputComponent->BindAction("Teleport", IE_Pressed, this, &ARogueCharacter::ProjectileTeleport);

	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &ARogueCharacter::SprintStart);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &ARogueCharacter::SprintStop);
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
	ActionComponent->StartActionByName(this, "PrimaryAttack");
}

void ARogueCharacter::SecondaryAttack() {
	ActionComponent->StartActionByName(this, "BlackHole");
}

void ARogueCharacter::ProjectileTeleport() {
	ActionComponent->StartActionByName(this, "Teleport");
}

void ARogueCharacter::PrimaryInteract() {
	InteractionComponent->PrimaryInteract();
}

FVector ARogueCharacter::GetPawnViewLocation() const {
	return CameraComponent->GetComponentLocation();
}

void ARogueCharacter::OnHealthChanged(AActor* InstigatorActor, class URogueAttributeComponent* OwningComponent, float NewHealth, float Delta) {
	if (Delta < 0.f) { // Took damage
		GetMesh()->SetScalarParameterValueOnMaterials(TimeToHitParamName, GetWorld()->GetTimeSeconds());
	}
	
	if (NewHealth <= 0.f && Delta <= 0.f) {
		APlayerController* PlayerController = Cast<APlayerController>(GetController());
		DisableInput(PlayerController);
		SetLifeSpan(5.f);
	}
}

void ARogueCharacter::HealSelf(float Amount/* = 100 */) {
	AttributeComponent->ApplyHealthChange(this, Amount);
}

// ABILITIES
void ARogueCharacter::SprintStart() {
	ActionComponent->StartActionByName(this, "Sprint");
}

void ARogueCharacter::SprintStop() {
	ActionComponent->StopActionByName(this, "Sprint");
}
