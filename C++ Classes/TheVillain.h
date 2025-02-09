// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "TheVillain.generated.h"



//UENUM(BlueprintType)
//enum class EViliainType : uint8 {
//
//	Default,
//	Idle
//
//};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FVillainBackToNormalState);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FChangeSpeed, float, NewSpeed);


class UAIPerceptionComponent;
class UAISenseConfig_Sight;

UCLASS()
class PACMAN_API ATheVillain : public APawn
{
	GENERATED_BODY()

		ATheVillain();

private:

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

public:

	UPROPERTY(BlueprintAssignable)
		FVillainBackToNormalState VillainBackToNormal;

	UFUNCTION()
		void BackToNormalState();

public:

	APawn* PlayerRef;
	class APacman_Char* PacmanRef;
	class APacman_Char* PacmanSensingRef;
	class ATheControForViliain* VilianAIConRef;

	//-----Game Mode Base Reference
	class APacmanGameModeBase* PacmanGameModeRef;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Developer Stuff")
		bool AbleToMove = true;

	UFUNCTION()
		void Hunt();

	UFUNCTION()
		void RotationTowardPlayer(FVector BaseLocation, FVector TargetLocation);


	//Speed Changing Has To be In Blueprint So I Made a delegate And +
	//The Cast Will Change The Speed in The Blueprint 
	UPROPERTY(BlueprintAssignable)
		FChangeSpeed ChangeSpeedCast;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float NormalSpeed = 320;

	UFUNCTION()
		void ToChangeSpeed(float NewSpeed);

	//-------------Speed Changing End -------------

private:

	// Sensing The player Functionalty 
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		UAIPerceptionComponent* AIPerc;
	UPROPERTY(EditAnywhere, BlueprintReadwrite, meta = (AllowPrivateAccess = "true"))
		UAISenseConfig_Sight* SightSense;

	UFUNCTION()
		void OnPerceptionUpdate(const TArray<AActor*>& UpdatedActors);

	UPROPERTY(EditAnywhere, BlueprintReadwrite, meta = (AllowPrivateAccess = "true"))
		bool PlayerSensed = false;
	//----------------

private:
	//-------Damage Box To Kill The player

	UPROPERTY(EditAnywhere, BlueprintReadwrite, meta = (AllowPrivateAccess = "true"))
		class UBoxComponent* AttackPlayerBox;

	UFUNCTION()
		void OnBeginOverlapKillBox(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(EditAnywhere, BlueprintReadwrite, meta = (AllowPrivateAccess = "true"))
		bool CantKillPlayer = false;

	//---------------------

	//-------Reach Point Movement Functionality 

	UFUNCTION()
		void MoveToReachPoint();

	UFUNCTION()
		void ToMoveToLastSeenLocation();
	int MoveTwice = 0;

private:
	//-----Getting Eaten Functionlity 

	UFUNCTION()
		void ScaredOfPlayer_EaterEvent(int EndTime);

	UFUNCTION()
		void RunAwayFromPlayer();
	class AActor* MainRP_Ref;

public:

	bool GettingEaten = false;

	FTimerHandle RunToRPTimer;
	FTimerManager TimerManager;
	///// Timer Test 

	float NumberIncreasing = 0;

	//------Getting Frozen By The Player 
	UFUNCTION()
		void Freeze(int EventEndTime);

	bool IsFrozen = false;

};
