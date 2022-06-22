// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "LostInMindGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class LOSTINMIND_API ALostInMindGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
protected:
	int LevelIndex = 0;
public:
	void LoadNextLevel();
	
};
