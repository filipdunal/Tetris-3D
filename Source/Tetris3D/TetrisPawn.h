// Filip Dunal 2022

#pragma once

#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "TetrisPawn.generated.h"


UCLASS()
class TETRIS3D_API ATetrisPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ATetrisPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		USpringArmComponent* SpringArm;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		UCameraComponent* Camera;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float fCameraSpeed;

	UPROPERTY()
	float fHorizontalAxis;

	UPROPERTY()
	float fVerticalAxis;

	bool bChunksMovingMode = true;

	void HandleCameraRotation();

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float CameraMovementTimeDelay;

	UPROPERTY()
		float CurrentCameraMovementTimeDelay;

	UPROPERTY()
		float DesiredCameraYaw;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float CameraMovementDeadzone;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		float TargetFOV;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void ReadVerticalAxis(float value);
	void ReadHorizontalAxis(float value);

	FORCEINLINE void EnableCameraMoving() { bChunksMovingMode = false; };
	FORCEINLINE void DisableCameraMoving() { bChunksMovingMode = true; };
};
