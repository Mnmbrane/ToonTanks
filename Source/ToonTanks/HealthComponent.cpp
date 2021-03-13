// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent.h"
#include "ToonTanks/GameModes/TankGameModeBase.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent() :
	mDefaultHealth(100.0f),
	mHealth(0.0f)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	mHealth = mDefaultHealth;
	mGameModeRef = Cast<ATankGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	GetOwner()->OnTakeAnyDamage.AddDynamic(this, &UHealthComponent::TakeDamage);
}

void UHealthComponent::TakeDamage(AActor* damagedActor, float damage, const UDamageType* damageType, AController* instigatedBy, AActor* damageCauser) 
{
	if(damage == 0 || mHealth <= 0)
	{
		return;
	}

	// Only take away damage when health is between 0 and the mDefaultHealth
	mHealth = FMath::Clamp(mHealth - damage, 0.0f, mDefaultHealth);

	if(mHealth <= 0)
	{
		if(mGameModeRef  != nullptr)
		{
			mGameModeRef->ActorDied(GetOwner());
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Pawn is still alive"));
		}
	}
}


