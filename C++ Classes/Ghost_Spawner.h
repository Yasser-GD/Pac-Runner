// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Ghost_Spawner.generated.h"

UCLASS()
class PACMAN_API AGhost_Spawner : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's propertiesz
	AGhost_Spawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;


private:
	class APawn* PlayerPawn;
	class APacman_Char* PacmanRef;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
		TSubclassOf<AActor> VillainRef;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
		int TimeToSpawn = 5.0;

	UFUNCTION()
		void SpawnWaitVillain(AController* AIController);
	UFUNCTION()
		void SpawnVillain();

};
