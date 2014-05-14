// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#include "Spellblade.h"
#include "SpellbladeCharacter.h"
#include "SpellbladeProjectile.h"

ASpellbladeCharacter::ASpellbladeCharacter(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
	// Set size for collision capsule
	CapsuleComponent->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate when the controller rotates.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Create a camera boom attached to the root (capsule)
	CameraBoom = PCIP.CreateDefaultSubobject<USpringArmComponent>(this, TEXT("CameraBoom"));
	CameraBoom->AttachTo(RootComponent);
	CameraBoom->bAbsoluteRotation = true; // Rotation of the character should not affect rotation of boom
	CameraBoom->TargetArmLength = 500.f;
	CameraBoom->SocketOffset = FVector(0.f,0.f,75.f);
	CameraBoom->RelativeRotation = FRotator(0.f,180.f,0.f);

	// Create a camera and attach to boom
	SideViewCameraComponent = PCIP.CreateDefaultSubobject<UCameraComponent>(this, TEXT("SideViewCamera"));
	SideViewCameraComponent->AttachTo(CameraBoom, USpringArmComponent::SocketName);
	SideViewCameraComponent->bUseControllerViewRotation = false; // We don't want the controller rotating the camera

	// Configure character movement
	CharacterMovement->bOrientRotationToMovement = true; // Face in the direction we are moving..
	CharacterMovement->RotationRate = FRotator(0.0f, 720.0f, 0.0f); // ...at this rotation rate
	CharacterMovement->GravityScale = 2.f;
	CharacterMovement->AirControl = 0.80f;
	CharacterMovement->JumpZVelocity = 1000.f;
	CharacterMovement->GroundFriction = 3.f;
	CharacterMovement->MaxWalkSpeed = 600.f;
	CharacterMovement->MaxFlySpeed = 600.f;

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)

	// Establish the offset for the initial firing position
	MuzzleOffset = FVector(0.0f, 0.0f, 0.0f);
}

//////////////////////////////////////////////////////////////////////////
// Input

void ASpellbladeCharacter::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	// set up gameplay key bindings
	InputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	InputComponent->BindAxis("MoveRight", this, &ASpellbladeCharacter::MoveRight);
	InputComponent->BindAction("Fire", IE_Pressed, this, &ASpellbladeCharacter::OnFire);
	InputComponent->BindTouch(IE_Pressed, this, &ASpellbladeCharacter::TouchStarted);
	InputComponent->BindAxis("AimVert", this, &ASpellbladeCharacter::SetAimVert);
	InputComponent->BindAxis("AimHorz", this, &ASpellbladeCharacter::SetAimHorz);
}

void ASpellbladeCharacter::MoveRight(float Value)
{
	// add movement in that direction
	AddMovementInput(FVector(0.f,-1.f,0.f), Value);
}

void ASpellbladeCharacter::TouchStarted(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	// jump on any touch
	Jump();
}

void ASpellbladeCharacter::SetAimVert(float value)
{
	Aim.Z = value;
	Aim.Normalize();
}

void ASpellbladeCharacter::SetAimHorz(float value)
{
	Aim.Y = value;
	Aim.Normalize();
}

void ASpellbladeCharacter::OnFire()
{
	//try to fire projectile
	if (ProjectileClass != NULL)
	{
		//get camera transforms
		FVector CameraLoc;
		FRotator CameraRot;
		GetActorEyesViewPoint(CameraLoc, CameraRot);

		//MuzzleOffset is in camera space, so transform to worldspace before offsetting from camera to fine final start position
		FVector const MuzzleLocation = CameraLoc + FTransform(CameraRot).TransformVector(MuzzleOffset);
		FRotator MuzzleRotation = CameraRot + FRotator(0.0f,-90.0f,0.0f);
		//tutorial says to skew aim upwards, but that's for flying

		UWorld* const World = GetWorld();
		if (World)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.Instigator = Instigator;

			//spawn projectile at muzzle
			ASpellbladeProjectile* Projectile = World->SpawnActor<ASpellbladeProjectile>(ProjectileClass, MuzzleLocation, MuzzleRotation, SpawnParams);
			if (Projectile)
			{
				if (Aim.IsNearlyZero()) {
					FVector default = GetActorRotation().Vector();
					default.X = 0;
					default.Normalize();
					Projectile->InitVelocity(default * Projectile->ProjectileMovement->InitialSpeed);
				}
				else {
					Projectile->InitVelocity(Aim * -(Projectile->ProjectileMovement->InitialSpeed));
				}
			}
		}
	}
}