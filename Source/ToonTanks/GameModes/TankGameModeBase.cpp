// Fill out your copyright notice in the Description page of Project Settings.


#include "TankGameModeBase.h"
#include "ToonTanks/Pawns/PawnTank.h"
#include "ToonTanks/Pawns/PawnTurret.h"
#include "Kismet/GameplayStatics.h"

void ATankGameModeBase::BeginPlay() 
{
   Super::BeginPlay();
 
   // Call HandleGameStart to initialise the turret counts
   // and also get the tank turret
   HandleGameStart();
}

int32 ATankGameModeBase::GetTargetTurretCount() 
{
   // Get the turret actors from the world
   TArray<AActor*> lTurretActors;
   UGameplayStatics::GetAllActorsOfClass(GetWorld(), APawnTurret::StaticClass(), lTurretActors);

   return lTurretActors.Num();
}

void ATankGameModeBase::ActorDied(AActor* deadActor) 
{
   const bool lPlayerLost = false;
   const bool lPlayerWon = true;
   // Check what type of Actor died. If turret, tally, If Player -> go to lose condition
   if(deadActor == mPlayerTank)
   {
      mPlayerTank->HandleDestruction();
      HandleGameOver(lPlayerLost);
   }
   else
   {
      APawnTurret* lDestroyedTurret = Cast<APawnTurret>(deadActor);
      // Make sure the dead actor is a PawnTurret
      if(lDestroyedTurret != nullptr)
      {
         lDestroyedTurret->HandleDestruction();
         mTargetTurrets--;
         if(mTargetTurrets <= 0)
         {
            HandleGameOver(lPlayerWon);
         }
      }
   }
}

void ATankGameModeBase::HandleGameStart() 
{
   mTargetTurrets = GetTargetTurretCount();
   // Get the player pawn
   // Using the index 0, since this is a single player game.
   mPlayerTank = Cast<APawnTank>(UGameplayStatics::GetPlayerPawn(this, 0));
   GameStart();
}

void ATankGameModeBase::HandleGameOver(bool playerWon) 
{
   GameOver(playerWon);
}
