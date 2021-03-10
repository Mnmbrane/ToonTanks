// Fill out your copyright notice in the Description page of Project Settings.


#include "PawnBase.h"
#include "Components/CapsuleComponent.h"
#include "ToonTanks/Actors/ProjectileBase.h"

// Sets default values
APawnBase::APawnBase()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create new components for this object
	mCapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule Collider"));
	RootComponent = mCapsuleComp;

	mBaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base Mesh"));
	mBaseMesh->SetupAttachment(RootComponent);

	mTurretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Turret Mesh"));
	mTurretMesh->SetupAttachment(mBaseMesh);

	mProjectileSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Projectile Spawn Point"));
	mProjectileSpawnPoint->SetupAttachment(mTurretMesh);
}

void APawnBase::RotateTurret(FVector lookAtTarget) 
{
	// Update TurretMesh rotation to face towards the LookAtTarget passed in from Child Classes
	
	// Use the lookAtTarget being passed in to create the vector
	// however only use the x and y rotation and not z since the turret doesn't need
	// to look up or down in this case
	FVector lLookAtTargetClean  = FVector(lookAtTarget.X, lookAtTarget.Y, mTurretMesh->GetComponentLocation().Z);
	FVector lStartLocation = mTurretMesh->GetComponentLocation();

	FRotator lTurretRotation = FVector(lLookAtTargetClean - lStartLocation).Rotation();
	mTurretMesh->SetWorldRotation(lTurretRotation);
}

void APawnBase::Fire() 
{
	// Get ProjectileSpawnPoint Location && Rotation -> Spawn Projectile class at Location firing towards Rotation
	if(ProjectileClass)
	{
		FVector lSpawnLocation = mProjectileSpawnPoint->GetComponentLocation();
		FRotator lSpawnRotation = mProjectileSpawnPoint->GetComponentRotation();
		AProjectileBase* lTempProjectile = 
			GetWorld()->SpawnActor<AProjectileBase>( ProjectileClass,
																  lSpawnLocation,
																  lSpawnRotation );

		// This will help later on to prevent collision with the pawn that is spawning
		// the projectile
		lTempProjectile->SetOwner(this);
	}
}

void APawnBase::HandleDestruction() 
{
	// Univeral Functionality
	// Play death effects particle, sound and camera shake

	// Then do child overrides
	// PawnTurret - Inform GameMode Turret died -> Then Destroy() self.

	// PawnTank - Inform GameMode Player died -> Then Hide() all components && stop movement input 
}
