// Fill out your copyright notice in the Description page of Project Settings.


#include "Ghost_Spawner.h"

#include "Kismet/GameplayStatics.h"
#include "Pacman_Char.h"
#include "TheVillain.h"
#include "TheControForViliain.h"

// Sets default values
AGhost_Spawner::AGhost_Spawner()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AGhost_Spawner::BeginPlay()
{
	Super::BeginPlay();

	PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	PacmanRef = Cast<APacman_Char>(PlayerPawn);
	if (PacmanRef) {
		PacmanRef->KilledGhostEvent.AddDynamic(this, &AGhost_Spawner::SpawnWaitVillain);
	}

}

// Called every frame
void AGhost_Spawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGhost_Spawner::SpawnWaitVillain(AController* AIController)
{

	FTimerHandle SpawnTimer;
	GetWorldTimerManager().SetTimer(SpawnTimer, this, &AGhost_Spawner::SpawnVillain, TimeToSpawn, false);

}

void AGhost_Spawner::SpawnVillain()
{

	if (VillainRef) {
		FActorSpawnParameters Params;
		Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		AActor* Villain = GetWorld()->SpawnActor<AActor>(VillainRef, GetActorLocation(), GetActorRotation(), Params);
		GEngine->AddOnScreenDebugMessage(-1, 7.0, FColor::Green, FString::Printf(TEXT("Supposd To Spawn ")));
	}

}

