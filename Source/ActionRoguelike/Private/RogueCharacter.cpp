// Fill out your copyright notice in the Description page of Project Settings.

#include "RogueCharacter.h"

#include "DrawDebugHelpers.h"
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

	// Attributes
	AttributeComponent = CreateDefaultSubobject<URogueAttributeComponent>(TEXT("AttributeComponent"));

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
	PlayerInputComponent->BindAction("SecondaryAttack", IE_Pressed, this, &ARogueCharacter::SecondaryAttack);
	PlayerInputComponent->BindAction("PrimaryInteract", IE_Pressed, this, &ARogueCharacter::PrimaryInteract);
	PlayerInputComponent->BindAction("Teleport", IE_Pressed, this, &ARogueCharacter::ProjectileTeleport);
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
	ProjectileAttack("Primary");
}

void ARogueCharacter::PrimaryAttack_TimeElapsed() {
	DoSpawnProjectile(PrimaryProjectileClass);
}

void ARogueCharacter::SecondaryAttack() {
	ProjectileAttack("Secondary");
}

void ARogueCharacter::SecondaryAttack_TimeElapsed() {
	DoSpawnProjectile(SecondaryProjectileClass);
}

void ARogueCharacter::ProjectileTeleport() {
	ProjectileAttack("Teleport");
}

void ARogueCharacter::ProjectileTeleport_TimeElapsed() {
	DoSpawnProjectile(TeleportProjectileClass);
}

void ARogueCharacter::ProjectileAttack(FString AttackType) {
	PlayAnimMontage(AttackAnimation);
	if (AttackType.Equals("Primary")) {
		GetWorldTimerManager().SetTimer(TimerHandle_PrimaryAttack, this, &ARogueCharacter::PrimaryAttack_TimeElapsed, .18f);
	} else if (AttackType.Equals("Secondary")) {
		GetWorldTimerManager().SetTimer(TimerHandle_SecondaryAttack, this, &ARogueCharacter::SecondaryAttack_TimeElapsed, .18f);
	} else if (AttackType.Equals("Teleport")) {
		GetWorldTimerManager().SetTimer(TimerHandle_ProjectileTeleport, this, &ARogueCharacter::ProjectileTeleport_TimeElapsed, .18f);
	}
}

void ARogueCharacter::PrimaryInteract() {
	InteractionComponent->PrimaryInteract();
}

FRotator ARogueCharacter::FindAimRotation() {
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (ensureAlways(PlayerController)) {
		FVector CrosshairWorldLocation, CrosshairWorldDirection;

		// Trace start
		int32 ViewportX, ViewportY;
		PlayerController->GetViewportSize(ViewportX, ViewportY);
		PlayerController->DeprojectScreenPositionToWorld(ViewportX/2, ViewportY/2, CrosshairWorldLocation, CrosshairWorldDirection);
		
		// Trace end
		const FVector TraceEnd = CameraComponent->GetForwardVector() * AttackDistance + CrosshairWorldLocation;
		
		// Objects
		FCollisionObjectQueryParams objectQueryParams;
		objectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);
		objectQueryParams.AddObjectTypesToQuery(ECC_WorldStatic);
		
		// Perform trace
		FHitResult hit;

		GetWorld()->LineTraceSingleByObjectType(hit, CrosshairWorldLocation, TraceEnd, objectQueryParams);
		
		return hit.bBlockingHit ?
			(hit.Location - GetMesh()->GetSocketLocation("Muzzle_01")).Rotation() :
			(TraceEnd - GetMesh()->GetSocketLocation("Muzzle_01")).Rotation();
	}
	return FRotator(0.f,0.f,0.f);
}

void ARogueCharacter::DoSpawnProjectile(TSubclassOf<AActor> ProjectileType) {
	FTransform SpawnTM = FTransform(FindAimRotation(), GetMesh()->GetSocketLocation("Muzzle_01"));

	FActorSpawnParameters spawnParams;
	spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	spawnParams.Instigator = this;
	
	GetWorld()->SpawnActor<AActor>(ProjectileType, SpawnTM, spawnParams);
}
