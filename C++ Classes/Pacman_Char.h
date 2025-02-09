// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Pacman_Char.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FAteGuideFoodie);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FBeTheEaterEvent, int, EndTime);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FKilledAGhost, AController*, AIController);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FFreezingEvent , int, EventEndTime);

class AFoodiee;

UENUM(BlueprintType)
enum class ECurrentAbility : uint8 {

	PowerUp = 0,
	GuideFoodie = 1 ,
	BeTheEater = 2,
	Freeze = 3,

};

UCLASS()
class PACMAN_API APacman_Char : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APacman_Char();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:

	//-----References
	class APacmanGameModeBase* PacmanGameModeRef;

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool PlayerKilled = false;

	UFUNCTION()
		void MoveRightLeft(float value);
	UFUNCTION()
		void MoveForwardBack(float value);

	UFUNCTION()
		void LookUpAndDown(float value);

protected:

	bool IsFrozen() { return Frozen; }

	UFUNCTION(BlueprintCallable)
		void setFrozen(bool Value) { Frozen = Value; }

public:

	void  SetDirction(const FVector Diraction);

	UPROPERTY(BlueprintReadOnly)
		bool Frozen = true;

private:

	UFUNCTION()
		void WhatFoodTypeConsumed(EFoodType FoodTypeConsumed);

	UFUNCTION()
		void OnOverlapBegin(class AActor* OverlappedActor, class AActor* OtherActor);

	UFUNCTION()
		void OnUse();
	UFUNCTION()
		void OnSlot_OnePressed();
	UFUNCTION()
		void OnSlot_TwoPressed();

	UFUNCTION()
		void MouseWheel_Up();

	UFUNCTION()
		void MouseWheel_Down();


private:

	//------------- Abilitys Section 

	UFUNCTION()
		void PowerUpAbility();

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
		float DefaultSpeed = 500;
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
		float PowerUpSpeed = 700;

	UPROPERTY(EditAnywhere, BlueprintReadwrite, meta = (AllowPrivateAccess = "true") , Category = "Foodiee Timer")
		int PowerUpEndTime = 8.0;

public:

	// Guide Foodie Broadcast 
	UPROPERTY(BlueprintAssignable)
		FAteGuideFoodie GuideFoodieEvent;

	// Be The Eater Foodiee Event 
	UPROPERTY(BlueprintAssignable)
		FBeTheEaterEvent BeTheEaterEvent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Foodiee Timer")
		int Eater_EventEndTimer = 13;

	UPROPERTY(BlueprintAssignable)
		FFreezingEvent FreezingEvent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Foodiee Timer")
	int FreezingEvnetEndTime = 10;


	// Eater Abillity State 

	UPROPERTY(EditAnywhere, BlueprintReadwrite, meta = (AllowPrivateAccess = "true"))
		class UBoxComponent* PlayerEaterBox;

	UPROPERTY(BlueprintAssignable)
		FKilledAGhost KilledGhostEvent;

	UFUNCTION()
		void GhostKillerBox(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);


	//----------------- To Manage Abilities Switching ----------------

	// Enum To Switch Between Abilities 
	UPROPERTY(BlueprintReadWrite)
		ECurrentAbility CurrentAbilityToUse;
	UPROPERTY(BlueprintReadWrite)
		ECurrentAbility Slot_One_AType;
	UPROPERTY(BlueprintReadWrite)
		ECurrentAbility Slot_Two_AType;

	// Number Of Foodiees Abilitys The Player Got 

	UPROPERTY(BlueprintReadWrite)
		int PowerUpNumber = 0;
	UPROPERTY(BlueprintReadWrite)
		int BeTheEaterNumber = 0;
	UPROPERTY(BlueprintReadWrite)
		int GuideAbilityNumber = 0;
	UPROPERTY(BlueprintReadWrite)
		int FreezeNumber = 0;

	// Booleans For Each Ability if On Use now
	UPROPERTY(BlueprintReadWrite)
		bool PowerUpOnUse = false;
	UPROPERTY(BlueprintReadWrite)
		bool GuideAbilityOnUse = false;
	UPROPERTY(BlueprintReadWrite)
		bool BeTheEaterOnUse = false;
	UPROPERTY(BlueprintReadWrite)
		bool FreezeOnUse = false;

	// Booleans For if Slot one Or Two Are on Use 
	UPROPERTY(BlueprintReadWrite)
		bool SlotOneOnUse = false;
	UPROPERTY(BlueprintReadWrite)
		bool SlotTwoOnUse = false;

	// Boolean if Any Ability Is On Use Now
	UPROPERTY(BlueprintReadWrite)
		bool OnUseAnyAbility = false;


	/// Count Down For Abilitys Varibles And Functions 
	UFUNCTION()
		void ToCountDown_SlotOne();
	UFUNCTION()
		void ToCountDown_SlotTwo();

	UPROPERTY(BlueprintReadWrite)
		int EndTimeFor_SlotOne = 0;
	UPROPERTY(BlueprintReadWrite)
		int EndTimeFor_SlotTwo = 0;

	int OriginalNumOne = 0;
	int OriginalNumTwo = 0;

	FTimerHandle CountDownTimerOne;
	FTimerHandle CountDownTimerTwo;

	bool GotResetOne = false;
	bool GotResetTwo = false;
};
