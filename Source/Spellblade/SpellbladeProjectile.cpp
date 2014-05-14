

#include "Spellblade.h"
#include "SpellbladeProjectile.h"


ASpellbladeProjectile::ASpellbladeProjectile(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
	// Use a sphere as a simple collision representation
	CollisionComp = PCIP.CreateDefaultSubobject<USphereComponent>(this, TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(5.0f);
	CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");			// Collision profiles are defined in DefaultEngine.ini
	CollisionComp->OnComponentHit.AddDynamic(this, &ASpellbladeProjectile::OnHit);		// set up a notification for when this component overlaps something
	RootComponent = CollisionComp;

	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement = PCIP.CreateDefaultSubobject<UProjectileMovementComponent>(this, TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 3000.f;
	ProjectileMovement->MaxSpeed = 3000.f;
	ProjectileMovement->bRotationFollowsVelocity = false;
	ProjectileMovement->bShouldBounce = true;

	// Die after 3 seconds by default
	InitialLifeSpan = 3.0f;
}

void ASpellbladeProjectile::OnHit(AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if ((OtherActor != NULL) && (OtherActor != this) && (OtherComp != NULL))
	{
		OtherComp->AddImpulseAtLocation(GetVelocity() * 100.0f, GetActorLocation());

		Destroy();
	}
}

FVector ASpellbladeProjectile::Reflect(FVector normal)
{
	FVector out = -(ProjectileMovement->Velocity);
	out.Normalize();
	float dot = out | normal;

	out.X = 2.f * normal.X * dot - out.X;
	out.Y = 2.f * normal.Y * dot - out.Y;
	out.Z = 2.f * normal.Z * dot - out.Z;

	return out;
}

bool ASpellbladeProjectile::SameInstigator(AActor* other)
{
	if (other)
		return Instigator == other->Instigator;
	else return false;
}

void ASpellbladeProjectile::InitVelocity(FVector const v)
{
	ProjectileMovement->Velocity = v;
}



