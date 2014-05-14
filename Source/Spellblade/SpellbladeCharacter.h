// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.
#pragma once

#include "SpellbladeCharacter.generated.h"

UCLASS(config=Game)
class ASpellbladeCharacter : public ACharacter
{
	GENERATED_UCLASS_BODY()

	/** Side view camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	TSubobjectPtr<UCameraComponent> SideViewCameraComponent;

	/** Camera boom positioning the camera beside the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
		TSubobjectPtr<class USpringArmComponent> CameraBoom;

	/** Offset for the projectile*/
	UPROPERTY(EditDefaultsOnly, Category=Gameplay)
		FVector MuzzleOffset;

	/** Aim vector */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Gameplay)
		FVector Aim;

	/** Projectile class to spawn */
	UPROPERTY(EditDefaultsOnly, Category = Projectile)
		TSubclassOf<class ASpellbladeProjectile> ProjectileClass;

protected:

	/** Called for side to side input */
	void MoveRight(float Val);

	/** Events upon firing */
	void OnFire();

	/** sets the vertical component of the aim vector */
	void SetAimVert(float value);

	/** sets the horizontal component of the aim vector */
	void SetAimHorz(float value);

	/** Handle touch inputs. */
	void TouchStarted(const ETouchIndex::Type FingerIndex, const FVector Location);

	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) OVERRIDE;
	// End of APawn interface
};
