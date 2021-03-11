// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileBase.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AProjectileBase::AProjectileBase() :
	mMovementSpeed(1300),
	mDamage(50)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	mProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Projectile Mesh"));

	// Bind when a mesh has been collided with to the function OnHit
	mProjectileMesh->OnComponentHit.AddDynamic(this, &AProjectileBase::OnHit);
	RootComponent = mProjectileMesh;

	// This is not part of the scene hierarchy or inherit any transform,
	// this does not need to be attached to anything 
	mProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement"));
	mProjectileMovement->InitialSpeed = mMovementSpeed;
	mProjectileMovement->MaxSpeed = mMovementSpeed;

	// How long the actor will live
	InitialLifeSpan = 3.0f;
}

// Called when the game starts or when spawned
void AProjectileBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void AProjectileBase::OnHit(UPrimitiveComponent* hitComp, AActor* otherActor, UPrimitiveComponent* otherComp, FVector normalImpulse, const FHitResult& hit) 
{
	// This is the pawn the spawns the projectile
	// Either the tank or turret in this case
	AActor* lMyOwner = GetOwner();

	if(lMyOwner == nullptr)
	{
		return;
	}
	
	if(otherActor != nullptr && otherActor != this && otherActor != lMyOwner)
	{
		UGameplayStatics::ApplyDamage(otherActor, mDamage, lMyOwner->GetInstigatorController(), this, DamageType);
	}

	// Destroy this actor once it's hit something
	Destroy();
}


