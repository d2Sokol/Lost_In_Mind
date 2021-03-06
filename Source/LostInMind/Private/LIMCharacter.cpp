// Fill out your copyright notice in the Description page of Project Settings.


#include "LIMCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "../Public/LevelGate.h"
#include "../Public/GateKey.h"
#include "../Public/Weapon.h"
#include "../Public/MeleeWeapon.h"
#include "../Public/PlayerInventory.h"
#include "../LostInMindGameModeBase.h"

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

	PlayerInventory = CreateDefaultSubobject<UPlayerInventory>(TEXT("PlayerInventoryComp"));

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
	PlayerInputComponent->BindAction("PickMeleeWeapon", IE_Pressed, this, &ALIMCharacter::PickWeaponFromInventory);
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
				if (EndLevelGate != nullptr) {
					EndLevelGate->SetGateWidgetVisibility(false);
					bIsInGate = false;
				}
			}

			if (AGateKey* Key = Cast<AGateKey>(Actor)) {
				Key->Destroy();
				PlayerKeysToGate++;
				EndLevelGate->RefreshWidget();
			}

			if (AWeapon* Weapon = Cast<AWeapon>(Actor)) {
				if (!Weapon->bPickedUp) {
					PlayerInventory->AddWeaponToInventory(Weapon);
					Weapon->SetActorEnableCollision(false);
					Weapon->SetActorHiddenInGame(true);
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
}

void ALIMCharacter::SetLevelGate()
{
	AActor* Gate = UGameplayStatics::GetActorOfClass(GetWorld(), LevelGateClass);

	if (Gate != nullptr) {
		EndLevelGate = Cast<ALevelGate>(Gate);
	}
}

void ALIMCharacter::EnterGate()
{
	if (bIsInGate) {
		if (EndLevelGate->CanEnterGate(PlayerKeysToGate)) {
			ALostInMindGameModeBase* GameMode = Cast<ALostInMindGameModeBase>(GetWorld()->GetAuthGameMode());
			GameMode->LoadNextLevel();
		}
		else {
			UE_LOG(LogTemp, Warning, TEXT("You don't have enough keys!"))
		}
	}
}

void ALIMCharacter::PickWeaponFromInventory(FKey Key)
{
	PlayerInventory->PickWeaponFromInventory(this, Key);
}

void ALIMCharacter::UseWeapon(AWeapon* Weapon)
{
	if (Weapon != nullptr) {
		Weapon->bPickedUp = true;
		Weapon->SetActorEnableCollision(true);
		Weapon->SetActorHiddenInGame(false);
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

int ALIMCharacter::GetNumberOfPlayerKeys()
{
	return PlayerKeysToGate;
}

int ALIMCharacter::GetNumberOfNeededKeys()
{
	if (EndLevelGate != nullptr) {
		return EndLevelGate->GetNeededKeys();
	}

	return 0;
}
