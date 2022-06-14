// Fill out your copyright notice in the Description page of Project Settings.


#include "LIMCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

// Sets default values
ALIMCharacter::ALIMCharacter()
{
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
		FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::X);
		AddMovementInput(Direction, Value/2.0f);
	}
}

void ALIMCharacter::TryJump()
{
	Jump();
}

