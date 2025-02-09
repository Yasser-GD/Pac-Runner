// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "PacmanGameModeBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FAllFoodieeConsumed);

UCLASS()
class PACMAN_API APacmanGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int FoodieeNumOnLevel;

	//Foodiee Consumed Count 
	UPROPERTY(EditAnywhere, BlueprintReadwrite)
		int FoodieeCollected = 0;

	UFUNCTION(BlueprintCallable)
		void AfterAllFoodiesConsumed();

	UPROPERTY(BlueprintAssignable)
		FAllFoodieeConsumed AllFoodieeConsumed;

	UPROPERTY(EditAnywhere, BlueprintReadwrite)
		bool IsAllFoodConsumed = false;

};
