// Fill out your copyright notice in the Description page of Project Settings.


#include "LIMCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

// Sets default values
ALIMCharacter::ALIMCharacter()
{
	Velocity = 0.0f;
	MaxVelocity = 0.5f;

	LIMCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	LIMCameraSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));

	LIMCameraSpringArm->SetupAttachment(RootComponent);
	LIMCamera->SetupAttachment(LIMCameraSpringArm);
}

// Called when the game starts or when spawned
void ALIMCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called to bind functionality to input
void ALIMCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveRight", this, &ALIMCharacter::MoveRight);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ALIMCharacter::TryJump);
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

