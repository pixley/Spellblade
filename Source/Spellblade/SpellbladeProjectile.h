

#pragma once

#include "GameFramework/Actor.h"
#include "SpellbladeProjectile.generated.h"

/**
 * 
 */
UCLASS(config = Game)
class ASpellbladeProjectile : public AActor
{
	GENERATED_UCLASS_BODY()

	/** Sphere collision component */
	UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
		TSubobjectPtr<USphereComponent> CollisionComp;

	/** Projectile movement component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement)
		TSubobjectPtr<class UProjectileMovementComponent> ProjectileMovement;

	/** called when projectile hits something */
	UFUNCTION()
		void OnHit(AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	/** generates a reflection vector */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = Projectile)
		FVector Reflect(FVector normal);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = Projectile)
		bool SameInstigator(AActor* other);

	/** initial velocity */
	void InitVelocity(FVector const v);
};
