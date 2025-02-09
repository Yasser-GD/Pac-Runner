// Fill out your copyright notice in the Description page of Project Settings.


#include "TheVillain.h"

#include "AIController.h"
#include "Pacman_Char.h"
#include "Kismet/GameplayStatics.h"
#include "TheControForViliain.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Components/BoxComponent.h"
#include "Reach_Point_ForVillain.h"
#include "TimerManager.h"
#include "PacmanGameModeBase.h"

ATheVillain::ATheVillain() {

	AIPerc = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerception"));
	SightSense = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight"));

	SightSense->DetectionByAffiliation.bDetectEnemies = true;
	SightSense->DetectionByAffiliation.bDetectNeutrals = true;
	SightSense->SightRadius = 1600.0;
	SightSense->LoseSightRadius = 1700;
	SightSense->PeripheralVisionAngleDegrees = 90.0;

	AIPerc->SetDominantSense(SightSense->GetSenseImplementation());
	AIPerc->ConfigureSense(*SightSense);

	//AttackPlayerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Kill Box"));

}


void ATheVillain::BeginPlay() {

	Super::BeginPlay();

	SpawnDefaultController();

	// Sensing The player Binding 
	AIPerc->OnPerceptionUpdated.AddDynamic(this, &ATheVillain::OnPerceptionUpdate);

	// Player Reference
	PlayerRef = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

	// AI Controller Reference
	VilianAIConRef = Cast<ATheControForViliain>(GetController());

	if (VilianAIConRef) {
		GEngine->AddOnScreenDebugMessage(-1, 7.0, FColor::Yellow, FString::Printf(TEXT("Ghost AIController Is Working Wellllllllll ")));
	}

	// Game Mode Base Reference
	PacmanGameModeRef = Cast<APacmanGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));

	FTimerHandle RP_Timer;
	GetWorldTimerManager().SetTimer(RP_Timer, this, &ATheVillain::MoveToReachPoint, 7.0, true);


	//----------- Pacman Be The Eater Event 
	PacmanRef = Cast<APacman_Char>(PlayerRef);
	if (PacmanRef) {
		PacmanRef->BeTheEaterEvent.AddDynamic(this, &ATheVillain::ScaredOfPlayer_EaterEvent);
		PacmanRef->FreezingEvent.AddDynamic(this, &ATheVillain::Freeze);
		//GEngine->AddOnScreenDebugMessage(-1, 7.0, FColor::Green, FString::Printf(TEXT("Pacman Ref For The Eater Is True ")));
	}

	VillainBackToNormal.AddDynamic(this, &ATheVillain::BackToNormalState);

	GEngine->AddOnScreenDebugMessage(-1, 7.0, FColor::Yellow, FString::Printf(TEXT("Begin Play is Working ")));
}

void ATheVillain::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	if (PlayerSensed) {
		RotationTowardPlayer(GetActorLocation(), PlayerRef->GetActorLocation());
	}

	if (PacmanGameModeRef && PacmanGameModeRef->IsAllFoodConsumed && AbleToMove) {
		Hunt();
	}


}

// To Rest The Villain To Normal State 
void ATheVillain::BackToNormalState()
{
	GettingEaten = false;
	CantKillPlayer = false;

	ToChangeSpeed(NormalSpeed);

	if (RunToRPTimer.IsValid()) {

		GetWorldTimerManager().ClearTimer(RunToRPTimer);
		GEngine->AddOnScreenDebugMessage(-1, 7.0, FColor::Yellow, FString::Printf(TEXT(" Cleared Timer  ")));
	}

	GEngine->AddOnScreenDebugMessage(-1, 7.0, FColor::Yellow, FString::Printf(TEXT(" Back To Normal State ")));

}

void ATheVillain::Hunt()
{
	if (VilianAIConRef && !IsFrozen && AbleToMove) {

		VilianAIConRef->MoveToActor(PlayerRef, -1, false);
	}

}

void ATheVillain::RotationTowardPlayer(FVector BaseLocation, FVector TargetLocation)
{
	FVector Distance = TargetLocation - BaseLocation;
	FRotator RotationTowardPlayer = Distance.Rotation();
	SetActorRotation(RotationTowardPlayer);

}

void ATheVillain::ToChangeSpeed(float NewSpeed)
{
	ChangeSpeedCast.Broadcast(NewSpeed);
}

void ATheVillain::OnPerceptionUpdate(const TArray<AActor*>& UpdatedActors)
{

	if (!GettingEaten) {

		for (AActor* SensedActor : UpdatedActors) {

			PacmanSensingRef = Cast<APacman_Char>(SensedActor);

		}
		if (PacmanSensingRef) {

			for (int y = 0; y < UpdatedActors.Num(); y++) {
				FActorPerceptionBlueprintInfo ActorPerInfo;
				AIPerc->GetActorsPerception(UpdatedActors[y], ActorPerInfo);
				if (ActorPerInfo.LastSensedStimuli[0].WasSuccessfullySensed()) {
					Hunt();
					PlayerSensed = true;
					GEngine->AddOnScreenDebugMessage(-1, 7.0, FColor::Yellow, FString::Printf(TEXT("Player Sensed  ")));
					MoveTwice = 0;
				}
				else {
					ToMoveToLastSeenLocation();
					PlayerSensed = false;
					GEngine->AddOnScreenDebugMessage(-1, 7.0, FColor::Yellow, FString::Printf(TEXT("Its Not THe player ")));
				}

			}

		}

	}

}

void ATheVillain::OnBeginOverlapKillBox(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	if (OtherActor) {

		APacman_Char* Pacman = Cast<APacman_Char>(OtherActor);

		if (Pacman && !CantKillPlayer && !Pacman->PlayerKilled) {
			GEngine->AddOnScreenDebugMessage(-1, 7.0, FColor::Yellow, FString::Printf(TEXT("player killedd ")));
			Pacman->PlayerKilled = true;
		}
	}

}

void ATheVillain::MoveToReachPoint()
{

	if (!PlayerSensed && GetVelocity().Length() == 0.0 && !GettingEaten && !IsFrozen && AbleToMove) {

		TArray<AActor*> ReachPoints;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AReach_Point_ForVillain::StaticClass(), ReachPoints);

		GEngine->AddOnScreenDebugMessage(-1, 7.0, FColor::Yellow, FString::Printf(TEXT("Its Not THe player : %i"), ReachPoints.Num()));
		int RandNum = FMath::RandRange(0, ReachPoints.Num()-1 );
		//GEngine->AddOnScreenDebugMessage(-1, 7.0, FColor::Yellow, FString::Printf(TEXT("Its Not THe player : %i"), RandNum));
		if (!ReachPoints.IsEmpty() && ReachPoints[RandNum]) {
			AReach_Point_ForVillain* Point = Cast<AReach_Point_ForVillain>(ReachPoints[RandNum]);

			if (Point) {
				FVector RP_Location = Point->GetActorLocation();
				if (VilianAIConRef) {
					VilianAIConRef->MoveToLocation(RP_Location);
				}
			}
		}
	};

}

void ATheVillain::ToMoveToLastSeenLocation()
{

	if (!IsFrozen && AbleToMove) {

		FVector LastPlayerLocation = PlayerRef->GetActorLocation();

		if (VilianAIConRef) {
			VilianAIConRef->MoveToLocation(LastPlayerLocation);
		}

		if (MoveTwice < 1) {
			FTimerHandle LLoction_Timer;
			GetWorldTimerManager().SetTimer(LLoction_Timer, this, &ATheVillain::ToMoveToLastSeenLocation, 6.0, false);
			MoveTwice++;
		}
	}
}


// To Run Away From Player Cuz He Will Try To Eat Him 
//+ The Changing Of Color Will Be On the Blueprint 
void ATheVillain::ScaredOfPlayer_EaterEvent(int EndTime)
{
	if (!GettingEaten) {

		GEngine->AddOnScreenDebugMessage(-1, 7.0, FColor::Green, FString::Printf(TEXT("Be The Eater Cast Works ")));
		GettingEaten = true;
		CantKillPlayer = true;

		ToChangeSpeed(420);
		//GEngine->AddOnScreenDebugMessage(-1, 7.0, FColor::Green, FString::Printf(TEXT("Be The Eater Cast Works After the Change of speed  ")));

		GetWorldTimerManager().SetTimer(RunToRPTimer, this, &ATheVillain::RunAwayFromPlayer, 1.0, true);

		FTimerHandle RunFromPlayerTimer;
		GetWorldTimerManager().SetTimer(RunFromPlayerTimer, [&]() {VillainBackToNormal.Broadcast(); }, EndTime, false);

	}
}


void ATheVillain::RunAwayFromPlayer()
{

	GEngine->AddOnScreenDebugMessage(-1, 7.0, FColor::Green, FString::Printf(TEXT("Getting A Far Away From Player Reach point ")));
	if (GetVelocity().Length() == 0.0) {

		TArray<AActor*> ReachPoints;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AReach_Point_ForVillain::StaticClass(), ReachPoints);
		double MainDistance = 0.0;
		//GEngine->AddOnScreenDebugMessage(-1, 7.0, FColor::Green, FString::Printf(TEXT("Got into Run Away  ")));
		//TArray<float> DistanceFromPlayer;

		for (AActor* RP_Ref : ReachPoints) {

			double RP_Distance = FVector::Dist(PlayerRef->GetActorLocation(), RP_Ref->GetActorLocation());
			//GEngine->AddOnScreenDebugMessage(-1, 7.0, FColor::Green, FString::Printf(TEXT("Got An Actor Distance Ref : %f"),RP_Distance ));
			if (MainDistance == 0.0) {
				MainDistance = RP_Distance;
				MainRP_Ref = RP_Ref;
			}
			else if (RP_Distance > MainDistance) {
				MainDistance = RP_Distance;
				MainRP_Ref = RP_Ref;
				//GEngine->AddOnScreenDebugMessage(-1, 7.0, FColor::Green, FString::Printf(TEXT("Got An Actor Distance Ref ")));
			}

		}
		if (MainRP_Ref) {
			AReach_Point_ForVillain* Point = Cast<AReach_Point_ForVillain>(MainRP_Ref);
			if (Point) {
				FVector RP_Location = Point->GetActorLocation();
				if (VilianAIConRef) {
					VilianAIConRef->MoveToLocation(RP_Location);
				}
			}
		}
	};



}

void ATheVillain::Freeze(int EventEndTime)
{

	VilianAIConRef->StopMovement();
	IsFrozen = true;

	FTimerHandle RunFromPlayerTimer;
	GetWorldTimerManager().SetTimer(RunFromPlayerTimer, [&]() {IsFrozen = false; PacmanRef->OnUseAnyAbility = false; PacmanRef->FreezeOnUse = false; }, EventEndTime, false);

}
