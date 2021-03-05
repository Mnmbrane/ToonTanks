// Fill out your copyright notice in the Description page of Project Settings.


#include "PawnTurret.h"
#include "PawnTank.h"
#include "Kismet/GameplayStatics.h"

APawnTurret::APawnTurret()
{

}

// Called when the game starts or when spawned
void APawnTurret::BeginPlay()
{
	Super::BeginPlay();
	
   // Get the world's timer instance and bind it to CheckFireCondition 
   GetWorldTimerManager().SetTimer(mFireRateTimerHandle, this, &APawnTurret::CheckFireCondition, mFireRate, true);

   mPlayerPawn = Cast<APawnTank>(UGameplayStatics::GetPlayerPawn(this, 0));

}

void APawnTurret::HandleDestruction() 
{
   // Call base pawn class HandleDestruction to play effects
   Super::HandleDestruction();
   Destroy();

}

// Called every frame
void APawnTurret::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
   
   if(mPlayerPawn == nullptr || ReturnDistanceToPlayer() > mFireRange)
   {
      return;
   }

   // Rotate the turret to the player
   RotateTurret(mPlayerPawn->GetActorLocation());
}

void APawnTurret::CheckFireCondition() 
{
   // If player == null || is dead
   if(mPlayerPawn == nullptr)
   {
      return;
   }

   // If plyaer is in range then fire
   if(ReturnDistanceToPlayer() <= mFireRange)
   {
      Fire();
   }
}

float APawnTurret::ReturnDistanceToPlayer() 
{
   // If player == null || is dead
   if(mPlayerPawn == nullptr)
   {
      return 0.0f;
   }

   // Find distance between this turret and the player tank
   return FVector::Dist(mPlayerPawn->GetActorLocation(), GetActorLocation());
}