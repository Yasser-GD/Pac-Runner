// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Pacman_GameInstance.generated.h"

/**
 *
 */
UCLASS()
class PACMAN_API UPacman_GameInstance : public UGameInstance
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = " Levels Names To Use")
		bool Level_One = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = " Levels Names To Use")
		bool Level_Two = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = " Levels Names To Use")
		bool Level_Three = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = " Levels Names To Use")
		bool Level_Four = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = " Levels Names To Use")
		bool GameCompleted = false;

public:

	//To Check if The player Has Abilitys 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = " Abilitys")
		bool PlayerCan_BombFood = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = " Abilitys")
		bool PlayerCan_ElectricFood = false;

};
