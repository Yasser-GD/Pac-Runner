// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/DataTable.h"
#include "Foodiee.generated.h"

class USoundCue;
class APacman_Char;

UENUM(BlueprintType)
enum class EFoodType :uint8 {

	Regular = 0,
	PowerUp = 1 ,
	GuideFoodie = 2 ,
	BeTheEater = 3 ,
	Freeze = 4 ,

};

USTRUCT(BlueprintType)
struct FFoodData : public FTableRowBase {

	GENERATED_USTRUCT_BODY()


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EFoodType FoodTypeData;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 FoodCollected;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UTexture2D* FoodImage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 UsageTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UBlueprint* FoodBlueprint;

};


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FFoodieEatenEvent, EFoodType, FoodieeType);

UCLASS()
class PACMAN_API AFoodiee : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AFoodiee();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	UFUNCTION(BlueprintCallable)
		void Consume();

	UPROPERTY(EditAnywhere , BlueprintReadWrite)
		EFoodType FoodieeType = EFoodType::Regular;

	UPROPERTY(EditAnywhere, Category = "Food Data")
		FFoodData FoodData;

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
		FFoodieEatenEvent FoodEvent;

private:

	//Player References
	APawn* PlayerRef;
	APacman_Char* PacmanRef;

	// Consuming Sound 
	UPROPERTY(EditAnywhere)
		USoundCue* ConsumptionSound;

	//468MmmM&&

public:

	UPROPERTY(EditAnywhere)
		bool AlreadyGettingPulled = false;

	UPROPERTY(EditAnywhere)
		float TimeElasped = 0.0;
	UPROPERTY(EditAnywhere)
		float LerpDuration = 2.0;
	UPROPERTY(EditAnywhere)
		float InterpSpeed = 20;

	UFUNCTION(BlueprintCallable)
		void GotPulled();

	UPROPERTY(EditAnywhere)
		bool GettingPulled = false;

};
