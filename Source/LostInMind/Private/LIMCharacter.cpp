// Fill out your copyright notice in the Description page of Project Settings.


#include "LIMCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/BoxComponent.h"
#include "../Public/LevelGate.h"

// Sets default values
ALIMCharacter::ALIMCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	LIMCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	LIMCameraSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));

	LIMCameraSpringArm->SetupAttachment(RootComponent);
	LIMCamera->SetupAttachment(LIMCameraSpringArm);
	
	InteractBox = CreateDefaultSubobject<UBoxComponent>(TEXT("InteractBox"));
	InteractBox->SetupAttachment(RootComponent);

	PlayerKeysToGate = 0;

}

// Called when the game starts or when spawned
void ALIMCharacter::BeginPlay()
{
	Super::BeginPlay();
	

}

void ALIMCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	sInteractBox();
}

// Called to bind functionality to input
void ALIMCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveRight", this, &ALIMCharacter::MoveRight);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ALIMCharacter::TryJump);
}

void ALIMCharacter::sInteractBox()
{
	TArray<AActor*> OverlappingActors;

	InteractBox->GetOverlappingActors(OverlappingActors);

	if (OverlappingActors.Num() != 0) {
		for (auto Actor : OverlappingActors) {
			EndLevelGate = Cast<ALevelGate>(Actor);
			if (EndLevelGate != nullptr) {
				EndLevelGate->SetGateWidgetVisibility(true);
				UE_LOG(LogTemp, Warning, TEXT("Vis true"));
			}
			else {
				UE_LOG(LogTemp, Warning, TEXT("Gate Not Found"));
				if (EndLevelGate != nullptr) {
					EndLevelGate->SetGateWidgetVisibility(false);
					EndLevelGate = nullptr;
				}
			}
		}
	}
	else {
		if (EndLevelGate != nullptr) {
			EndLevelGate->SetGateWidgetVisibility(false);
			EndLevelGate = nullptr;
		}
	}
}

void ALIMCharacter::MoveRight(float Value)
{
	if (Value != 0.0f) {
	
		FVector WorldDirection = {1.0f, 0.0f, 0.0f};

		if (Value == 1.0f) {
			MovementState = EMovementState::MOVEMENT_WALK_RIGHT;
		}
		else {
			MovementState = EMovementState::MOVEMENT_WALK_LEFT;
		}

		AddMovementInput(WorldDirection, Value/2.0f);

	}
}

void ALIMCharacter::TryJump()
{
	MovementState = EMovementState::MOVEMENT_JUMP;
	Jump();
}

const FVector ALIMCharacter::GetCurrentVelocity() const
{
	return GetVelocity();
}

const EMovementState ALIMCharacter::GetMovementState() const
{
	return MovementState;
}

FName ALIMCharacter::GetGateWidgetText()
{
	return FName("Keys: "+PlayerKeysToGate);
}


