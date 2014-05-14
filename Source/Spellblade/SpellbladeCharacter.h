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

	/** Projectile class to spawn */
	UPROPERTY(EditDefaultsOnly, Category = Projectile)
		TSubclassOf<class ASpellbladeProjectile> ProjectileClass;

	/** Events upon firing */
	UFUNCTION()
		void OnFire();

protected:

	/** Called for side to side input */
	void MoveRight(float Val);

	/** Handle touch inputs. */
	void TouchStarted(const ETouchIndex::Type FingerIndex, const FVector Location);

	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) OVERRIDE;
	// End of APawn interface
};
