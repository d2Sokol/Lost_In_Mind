// Copyright Epic Games, Inc. All Rights Reserved.


#include "LostInMindGameModeBase.h"
#include "Kismet/GameplayStatics.h"

void ALostInMindGameModeBase::LoadNextLevel()
{

	LevelIndex++;

	UGameplayStatics::OpenLevel(GetWorld(), FName("LIM_Level_" + FString::FromInt(LevelIndex)));
}
