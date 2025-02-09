// Copyright Epic Games, Inc. All Rights Reserved.


#include "PacmanGameModeBase.h"

#include "Kismet/GameplayStatics.h"
#include "Foodiee.h"

void APacmanGameModeBase::BeginPlay() {

	Super::BeginPlay();

	TArray<AActor*> Foodiees;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AFoodiee::StaticClass(), Foodiees);
	FoodieeNumOnLevel = Foodiees.Num();

}


void APacmanGameModeBase::AfterAllFoodiesConsumed() {

	AllFoodieeConsumed.Broadcast();
	IsAllFoodConsumed = true;

}