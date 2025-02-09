// Fill out your copyright notice in the Description page of Project Settings.


#include "Foodiee.h"

#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "Pacman_Char.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
AFoodiee::AFoodiee()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AFoodiee::BeginPlay()
{
	Super::BeginPlay();

	PlayerRef = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	PacmanRef = Cast<APacman_Char>(PlayerRef);
	/*if (PacmanRef) {
		GEngine->AddOnScreenDebugMessage(-1, 7.0, FColor::Red, FString::Printf(TEXT("Pacman Reference is True ")));
	}*/

}

// Called every frame
void AFoodiee::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (GettingPulled) {
		SetActorLocation(FMath::VInterpTo(GetActorLocation(), PacmanRef->GetActorLocation(), DeltaTime, InterpSpeed));
	}

}

void AFoodiee::Consume()
{
	UGameplayStatics::PlaySound2D(this, ConsumptionSound);
	FoodEvent.Broadcast(FoodieeType);
	this->Destroy();

}

void AFoodiee::GotPulled()
{
	GettingPulled = true;

}

