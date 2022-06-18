// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "LIMCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class UBoxComponent;
class ALevelGate;

enum EMovementState {
	MOVEMENT_IDLE = 0,
	MOVEMENT_WALK_RIGHT,
	MOVEMENT_WALK_LEFT,
	MOVEMENT_CROUCH,
	MOVEMENT_JUMP
};

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
	virtual void Tick(float DeltaSeconds);

public:
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	int PlayerKeysToGate;

protected:

	EMovementState MovementState;

	UPROPERTY(EditAnywhere)
		UCameraComponent* LIMCamera;

	UPROPERTY(EditAnywhere)
		USpringArmComponent* LIMCameraSpringArm;

	UPROPERTY(EditAnywhere)
		UBoxComponent* InteractBox;

	ALevelGate* EndLevelGate;

protected:

	void sInteractBox();

	void MoveRight(float Value);
	void TryJump();

public:

	const FVector GetCurrentVelocity() const;

	const EMovementState GetMovementState() const;

	UFUNCTION(BlueprintCallable)
		FName GetGateWidgetText();

};
