// Filip Dunal 2022

#pragma once

#include "TetrisGameInstance.h"
#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "TetrisSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class TETRIS3D_API UTetrisSaveGame : public USaveGame
{
	GENERATED_BODY()

public:

	UPROPERTY(VisibleAnywhere)
		TArray<FLevelSaveStruct> LevelsSave;

};
