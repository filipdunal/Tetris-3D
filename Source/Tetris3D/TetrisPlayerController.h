// Filip Dunal 2022

#pragma once

#include "GroupOfChunks.h"
#include "TetrisGameMode.h"
#include "Chunk.h"
#include "TetrisPawn.h"
#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TetrisPlayerController.generated.h"

UENUM(BlueprintType)
enum class CameraDirection : uint8 {
	SOUTH = 0   UMETA(DisplayName = "SOUTH"),
	WEST = 1    UMETA(DisplayName = "WEST"),
	NORTH = 2   UMETA(DisplayName = "NORTH"),
	EAST = 3    UMETA(DisplayName = "EAST")
};

/**
 * 
 */
UCLASS()
class TETRIS3D_API ATetrisPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	
	
	// PROPERTIES

	UPROPERTY()
		float fHorizontalAxis;

	UPROPERTY()
		float fVerticalAxis;

	UPROPERTY()
		ATetrisPawn* TetrisPawn;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float MovementTimeDelay;

	UPROPERTY()
		float CurrentMovementTimeDelay;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float MovementDeadzone;

	UPROPERTY()
		bool bChunksMovingMode;

	UPROPERTY()
		CameraDirection CamDirection;

	UPROPERTY()
		ATetrisGameMode* TetrisGameMode;

	// FUNCTIONS

	UFUNCTION()
		FVector GetDesiredMove();

	UFUNCTION()
		AGroupOfChunks* GetGroupOfChunks();

	FORCEINLINE void ReadVerticalAxis(float value) { fVerticalAxis = value;  };
	FORCEINLINE void ReadHorizontalAxis(float value) { fHorizontalAxis = value; };

	UFUNCTION()
		void HandleChunksMoving();

	UFUNCTION()
		void UpdateCameraDirection();

	UFUNCTION()
		void EnableChunksMoving();

	UFUNCTION()
		void DisableChunksMoving();

	UFUNCTION()
		bool IsMovePossible(FVector Direction, AGroupOfChunks* GroupOfChunks);

	UFUNCTION()
		void RotateChunksX();

	UFUNCTION()
		void RotateChunksY();

	UFUNCTION()
		void RotateChunksZ();

	UFUNCTION()
		void LandGroupOfChunks();

public:
	virtual void Tick(float DeltaTime) override;
	
};
