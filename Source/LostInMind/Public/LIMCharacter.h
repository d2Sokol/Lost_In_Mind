// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "LIMCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;

UCLASS()
class LOSTINMIND_API ALIMCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ALIMCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	UPROPERTY(EditAnywhere)
	UCameraComponent* LIMCamera;

	UPROPERTY(EditAnywhere)
	USpringArmComponent* LIMCameraSpringArm;

protected:

	void MoveRight(float Value);
	void TryJump();

};
