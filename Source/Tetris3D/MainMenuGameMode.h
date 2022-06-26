// Filip Dunal 2022

#pragma once

#include "MainMenu.h"
#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MainMenuGameMode.generated.h"

/**
 * 
 */
UCLASS()
class TETRIS3D_API AMainMenuGameMode : public AGameModeBase
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadWrite)
		UMainMenu* MainMenuWidgetRef;
};
