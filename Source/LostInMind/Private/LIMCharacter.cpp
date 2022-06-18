// Fill out your copyright notice in the Description page of Project Settings.


#include "LIMCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/BoxComponent.h"
#include "../Public/LevelGate.h"
#include "Kismet/GameplayStatics.h"

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
	bIsInGate = false;

}

// Called when the game starts or when spawned
void ALIMCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	SetLevelGate();
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
	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &ALIMCharacter::EnterGate);
}

void ALIMCharacter::sInteractBox()
{
	TArray<AActor*> OverlappingActors;

	InteractBox->GetOverlappingActors(OverlappingActors);

	if (OverlappingActors.Num() != 0) {
		for (auto Actor : OverlappingActors) {
			if (EndLevelGate != nullptr) {
				EndLevelGate->SetGateWidgetVisibility(true);
				bIsInGate = true;
			}
			else {
				UE_LOG(LogTemp, Warning, TEXT("Gate Not Found"));
				if (EndLevelGate != nullptr) {
					EndLevelGate->SetGateWidgetVisibility(false);
					bIsInGate = false;
				}
			}
		}
	}
	else {
		if (EndLevelGate != nullptr) {
			EndLevelGate->SetGateWidgetVisibility(false);
			bIsInGate = true;
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
	PlayerKeysToGate++;
}

void ALIMCharacter::SetLevelGate()
{
	TArray<AActor*> LevelGates;

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), LevelGateClass, LevelGates);

	for (auto LevelGate : LevelGates) {
		EndLevelGate = Cast<ALevelGate>(LevelGate);
	}
}

void ALIMCharacter::EnterGate()
{
	if (bIsInGate) {
		if (EndLevelGate->CanEnterGate(PlayerKeysToGate)) {
			UE_LOG(LogTemp, Warning, TEXT("Entering gate..."))
		}
		else {
			UE_LOG(LogTemp, Warning, TEXT("You don't have enough keys!"))
		}
	}
}

const FVector ALIMCharacter::GetCurrentVelocity() const
{
	return GetVelocity();
}

const EMovementState ALIMCharacter::GetMovementState() const
{
	return MovementState;
}

int ALIMCharacter::GetNumberOfNeededKeys()
{
	if (EndLevelGate != nullptr) {
		return EndLevelGate->GetNeededKeys();
	}

	return 0;
}

FName ALIMCharacter::GetGateWidgetText()
{	
	return FName(FString::FromInt(PlayerKeysToGate)+"/"+FString::FromInt(GetNumberOfNeededKeys()));
}


