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
	
}

// Called every frame
void APawnTank::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

   Rotate();
   Move();
}

// Called to bind functionality to input
void APawnTank::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

   PlayerInputComponent->BindAxis("MoveForward", this, &APawnTank::CalculateMoveInput);
   PlayerInputComponent->BindAxis("Turn", this, &APawnTank::CalculateRotateInput);
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


