// Fill out your copyright notice in the Description page of Project Settings.


#include "PawnTank.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

APawnTank::APawnTank() :
   mMoveSpeed(100.0f),
   mRotateSpeed(100.0f)
{
   mSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
   mSpringArm->SetupAttachment(RootComponent);

   mCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
   mCamera->SetupAttachment(mSpringArm);
}

// Called when the game starts or when spawned
void APawnTank::BeginPlay()
{
	Super::BeginPlay();
	
   // We can get a reference to the Player controller by casting the AController we get from
   // GetController() to a APlayerController and storing that as an APlayerController*
   mPlayerControllerRef = Cast<APlayerController>(GetController());
}

void APawnTank::HandleDestruction() 
{
   Super::HandleDestruction();
   // Hide player, TODO - Create new function to handle this

}

// Called every frame
void APawnTank::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

   Rotate();
   Move();

   if(mPlayerControllerRef != nullptr)
   {
      FHitResult lTraceHitResult;

      // Trace from the location on the screen is currently detected into the world space and find the resulting location
      // This will then be saved into the FHitResult
      mPlayerControllerRef->GetHitResultUnderCursor(ECC_Visibility, false, lTraceHitResult);

      // The location in the world that the trace has hit
      FVector lHitLocation = lTraceHitResult.ImpactPoint;

      // Rotate the tank to the calculated hit result.
      RotateTurret(lHitLocation);
   }
}

// Called to bind functionality to input
void APawnTank::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

   PlayerInputComponent->BindAxis("MoveForward", this, &APawnTank::CalculateMoveInput);
   PlayerInputComponent->BindAxis("Turn", this, &APawnTank::CalculateRotateInput);
   PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &APawnTank::Fire);
}

void APawnTank::CalculateMoveInput(float value) 
{
   mMoveDirection = FVector(value * mMoveSpeed * GetWorld()->DeltaTimeSeconds, 0, 0);
}

void APawnTank::CalculateRotateInput(float value) 
{
   float lRotateAmount = value * mRotateSpeed * GetWorld()->DeltaTimeSeconds;
   FRotator lRotation = FRotator(0, lRotateAmount, 0);
   mRotationDirection = FQuat(lRotation);
}

void APawnTank::Move() 
{
   AddActorLocalOffset(mMoveDirection, true);
}

void APawnTank::Rotate() 
{
   AddActorLocalRotation(mRotationDirection, true);
}


