// Fill out your copyright notice in the Description page of Project Settings.


#include "Pacman_Char.h"

#include "Foodiee.h"
#include "PacmanGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/BoxComponent.h"
#include "TheVillain.h"

// Sets default values
APacman_Char::APacman_Char()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PlayerEaterBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Kill Box"));
	PlayerEaterBox->SetupAttachment(GetMesh());

}

// Called when the game starts or when spawned
void APacman_Char::BeginPlay()
{
	Super::BeginPlay();

	OnActorBeginOverlap.AddDynamic(this, &APacman_Char::OnOverlapBegin);

	PacmanGameModeRef = Cast<APacmanGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));

	TArray<AActor*> Foodiees;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AFoodiee::StaticClass(), Foodiees);

	for (AActor* TheFood : Foodiees) {
		if (TheFood->ActorHasTag("Food")) {
			AFoodiee* FoodRef = Cast<AFoodiee>(TheFood);

			if (FoodRef->FoodieeType != EFoodType::Regular) {
				FoodRef->FoodEvent.AddDynamic(this, &APacman_Char::WhatFoodTypeConsumed);
			}
		}

	}

	PlayerEaterBox->OnComponentBeginOverlap.AddDynamic(this, &APacman_Char::GhostKillerBox);

}

// Called every frame
void APacman_Char::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APacman_Char::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{

	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("Slot_One", IE_Pressed, this, &APacman_Char::OnSlot_OnePressed);
	PlayerInputComponent->BindAction("Slot_Two", IE_Pressed, this, &APacman_Char::OnSlot_TwoPressed);

	PlayerInputComponent->BindAction("OnUse", IE_Pressed, this, &APacman_Char::OnUse);

	PlayerInputComponent->BindAxis("Right/LeftMove", this, &APacman_Char::MoveRightLeft);
	PlayerInputComponent->BindAxis("Back/Forward", this, &APacman_Char::MoveForwardBack);

	PlayerInputComponent->BindAxis("LookRight/Left", this, &ACharacter::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp/Down", this, &APacman_Char::LookUpAndDown);

	PlayerInputComponent->BindAction("Mouse Wheel Up", IE_Pressed, this, &APacman_Char::MouseWheel_Up);
	PlayerInputComponent->BindAction("Mouse Wheel Down", IE_Pressed, this, &APacman_Char::MouseWheel_Down);

}

void APacman_Char::MoveRightLeft(float value)
{
	if (!PlayerKilled) {
		AddMovementInput(GetActorRightVector(), value);
	}

}

void APacman_Char::MoveForwardBack(float value)
{
	if (!PlayerKilled) {
		AddMovementInput(GetActorForwardVector(), value);
	}
}

void APacman_Char::LookUpAndDown(float value)
{

	if (value != 0.f && Controller && Controller->IsLocalPlayerController() && PacmanGameModeRef && PacmanGameModeRef->IsAllFoodConsumed)
	{
		APlayerController* const PC = CastChecked<APlayerController>(Controller);
		PC->AddPitchInput(value);
	}

}

void APacman_Char::SetDirction(const FVector Diraction)
{
}

// To See if The player Picked Up A Strange Type of food 
void APacman_Char::WhatFoodTypeConsumed(EFoodType FoodTypeConsumed)
{
	if (FoodTypeConsumed == EFoodType::PowerUp) {
		PowerUpNumber++;
		GEngine->AddOnScreenDebugMessage(-1, 7.0, FColor::Purple, FString::Printf(TEXT("Power Up : %i "), PowerUpNumber));

	}
	else if (FoodTypeConsumed == EFoodType::GuideFoodie) {
		GuideAbilityNumber++;
		GEngine->AddOnScreenDebugMessage(-1, 7.0, FColor::Purple, FString::Printf(TEXT("Be The Eater : %i "), GuideAbilityNumber));
	}
	else if (FoodTypeConsumed == EFoodType::BeTheEater) {
		GEngine->AddOnScreenDebugMessage(-1, 7.0, FColor::Green, FString::Printf(TEXT("Casting Eater")));
		BeTheEaterNumber++;
	}
	else if (FoodTypeConsumed == EFoodType::Freeze) {
		FreezeNumber++;
	}

}

void APacman_Char::OnOverlapBegin(AActor* OverlappedActor, AActor* OtherActor)
{

	GEngine->AddOnScreenDebugMessage(-1, 1.0, FColor::Cyan, FString::Printf(TEXT("FOoooooood")));
	if (OtherActor->ActorHasTag("Food")) {

		AFoodiee* TheFood = Cast<AFoodiee>(OtherActor);
		TheFood->Consume();

		//TheFood->FoodEvent.AddDynamic(this, &APacman_Char::WhatFoodTypeConsumed);
		if (PacmanGameModeRef) {
			PacmanGameModeRef->FoodieeCollected++;
			if (PacmanGameModeRef->FoodieeCollected == PacmanGameModeRef->FoodieeNumOnLevel) {
				GEngine->AddOnScreenDebugMessage(-1, 7.0, FColor::Cyan, FString::Printf(TEXT("In C++ All Foodiees Consumed ")));
				PacmanGameModeRef->AfterAllFoodiesConsumed();
			}

		}
		//TheFood->FoodEvent.AddDynamic(this, APacman_Char::WhatFoodTypeConsumed);
	}

}

void APacman_Char::OnUse()
{

	if (PowerUpNumber > 0 && CurrentAbilityToUse == ECurrentAbility::PowerUp && !PowerUpOnUse) {
		PowerUpAbility();
		PowerUpOnUse = true;
		OnUseAnyAbility = true;
		ToCountDown_SlotOne();
		ToCountDown_SlotTwo();
		PowerUpNumber--;
		//GEngine->AddOnScreenDebugMessage(-1, 7.0, FColor::Purple, FString::Printf(TEXT("Power Up : %i "), PowerUpNumber));
	}
	else if (GuideAbilityNumber > 0 && CurrentAbilityToUse == ECurrentAbility::GuideFoodie && !GuideAbilityOnUse) {

		//To BroadCast And say that he using guide Food Event
		GuideFoodieEvent.Broadcast();

		GuideAbilityOnUse = true;
		OnUseAnyAbility = true;
		ToCountDown_SlotOne();
		ToCountDown_SlotTwo();
		GuideAbilityNumber--;
		//GEngine->AddOnScreenDebugMessage(-1, 7.0, FColor::Purple, FString::Printf(TEXT("Be The Eater : %i "), GuideAbilityNumber));
	}
	else if (BeTheEaterNumber > 0 && CurrentAbilityToUse == ECurrentAbility::BeTheEater && !BeTheEaterOnUse) {

		//To BroadCast And say that he using Be The Eater Food Event
		BeTheEaterEvent.Broadcast(Eater_EventEndTimer);

		BeTheEaterOnUse = true;
		OnUseAnyAbility = true;
		ToCountDown_SlotOne();
		ToCountDown_SlotTwo();
		BeTheEaterNumber--;
		//GEngine->AddOnScreenDebugMessage(-1, 7.0, FColor::Purple, FString::Printf(TEXT("Guide Foodiee: %i "), BeTheEaterNumber));
	}
	else if (FreezeNumber > 0 && CurrentAbilityToUse == ECurrentAbility::Freeze && !FreezeOnUse) {

		FreezingEvent.Broadcast(FreezingEvnetEndTime);
		ToCountDown_SlotOne();
		ToCountDown_SlotTwo();
		FreezeNumber--;
		OnUseAnyAbility = true;
		FreezeOnUse = true;
	}

}

void APacman_Char::OnSlot_OnePressed()
{
	//if (CurrentAbilityToUse != Slot_One_AType) {
	CurrentAbilityToUse = Slot_One_AType;
	SlotOneOnUse = true;
	GEngine->AddOnScreenDebugMessage(-1, 7.0, FColor::Green, FString::Printf(TEXT("Assigned Current Ability To Another One : %i"), CurrentAbilityToUse));

	//}

}

void APacman_Char::OnSlot_TwoPressed()
{
	//if (CurrentAbilityToUse != Slot_One_AType) {
	CurrentAbilityToUse = Slot_Two_AType;
	SlotTwoOnUse = true;
	GEngine->AddOnScreenDebugMessage(-1, 7.0, FColor::Green, FString::Printf(TEXT("Assigned Current Ability To Another Two : %i"), CurrentAbilityToUse));
	//}
}


void APacman_Char::MouseWheel_Up()
{

	if (CurrentAbilityToUse == Slot_Two_AType) {

		CurrentAbilityToUse = Slot_One_AType;
		SlotOneOnUse = true;
		GEngine->AddOnScreenDebugMessage(-1, 7.0, FColor::Green, FString::Printf(TEXT("Assigned Current Ability To Another One : %i"), CurrentAbilityToUse));

	}else if (CurrentAbilityToUse == Slot_One_AType) {

		CurrentAbilityToUse = Slot_Two_AType;
		SlotTwoOnUse = true;
		GEngine->AddOnScreenDebugMessage(-1, 7.0, FColor::Green, FString::Printf(TEXT("Assigned Current Ability To Another One : %i"), CurrentAbilityToUse));

	}

}


void APacman_Char::MouseWheel_Down()
{

	if (CurrentAbilityToUse == Slot_Two_AType) {

		CurrentAbilityToUse = Slot_One_AType;
		SlotOneOnUse = true;
		GEngine->AddOnScreenDebugMessage(-1, 7.0, FColor::Green, FString::Printf(TEXT("Assigned Current Ability To Another One : %i"), CurrentAbilityToUse));

	}else if (CurrentAbilityToUse == Slot_One_AType) {

		CurrentAbilityToUse = Slot_Two_AType;
		SlotTwoOnUse = true;
		GEngine->AddOnScreenDebugMessage(-1, 7.0, FColor::Green, FString::Printf(TEXT("Assigned Current Ability To Another One : %i"), CurrentAbilityToUse));

	}

}


void APacman_Char::PowerUpAbility()
{

	GetCharacterMovement()->MaxWalkSpeed = PowerUpSpeed;

	FTimerHandle PowerUpTimer;
	GetWorldTimerManager().SetTimer(PowerUpTimer, [&]() {GetCharacterMovement()->MaxWalkSpeed = DefaultSpeed; PowerUpOnUse = false; OnUseAnyAbility = false; }, PowerUpEndTime, false);

}


void APacman_Char::GhostKillerBox(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	if (OtherActor) {
		ATheVillain* VillainRef = Cast<ATheVillain>(OtherActor);
		if (VillainRef && VillainRef->GettingEaten) {
			KilledGhostEvent.Broadcast(VillainRef->GetController());

			/*FTimerHandle Timer;
			GetWorldTimerManager().SetTimer(Timer, [&]() {BeTheEaterEvent.Broadcast(VillainRef->NumberIncreasing); }, 1.0, false);*/

			VillainRef->Destroy();

		}
	}

}


void APacman_Char::ToCountDown_SlotOne()
{

	if (CurrentAbilityToUse == Slot_One_AType) {

		GotResetOne = true;

		if (OriginalNumOne == 0) {
			OriginalNumOne = EndTimeFor_SlotOne;
		}

		GEngine->AddOnScreenDebugMessage(-1, 7.0, FColor::Green, FString::Printf(TEXT("Assigned Current Ability To Another Two : %i"), OriginalNumOne));

		if (!CountDownTimerOne.IsValid()) {
			GEngine->AddOnScreenDebugMessage(-1, 7.0, FColor::Cyan, FString::Printf(TEXT("Started Slot One Timer ")));
			GetWorldTimerManager().SetTimer(CountDownTimerOne, [&]() { if (EndTimeFor_SlotOne > 0 && GotResetOne) { EndTimeFor_SlotOne--; } else if (EndTimeFor_SlotOne == 0) { EndTimeFor_SlotOne = OriginalNumOne; GotResetOne = false; } }, 1.0, true);
		}
	}

}


void APacman_Char::ToCountDown_SlotTwo()
{

	if (CurrentAbilityToUse == Slot_Two_AType) {

		GotResetTwo = true;

		if (OriginalNumTwo == 0) {
			OriginalNumTwo = EndTimeFor_SlotTwo;
		}

		if (!CountDownTimerTwo.IsValid()) {
			GEngine->AddOnScreenDebugMessage(-1, 7.0, FColor::Cyan, FString::Printf(TEXT("Started Slot Two Timer ")));
			GetWorldTimerManager().SetTimer(CountDownTimerTwo, [&]() { if (EndTimeFor_SlotTwo > 0 && GotResetTwo) { EndTimeFor_SlotTwo--; } else if (EndTimeFor_SlotTwo == 0) { EndTimeFor_SlotTwo = OriginalNumTwo; GotResetTwo = false; } }, 1.0, true);
		}
	}

}

