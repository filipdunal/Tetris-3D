// Filip Dunal 2022

#include "Kismet/KismetMathLibrary.h"
#include "TetrisPawn.h"
#include "Components/InputComponent.h"

// Sets default values
ATetrisPawn::ATetrisPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	RootComponent = SpringArm;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->AttachTo(RootComponent);

	//Super::SetupPlayerInputComponent(InputComponent);
	
}

void ATetrisPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ATetrisPawn::ReadVerticalAxis(float value)
{
	fVerticalAxis = value;
}

void ATetrisPawn::ReadHorizontalAxis(float value)
{
	fHorizontalAxis = value;
}

// Called when the game starts or when spawned
void ATetrisPawn::BeginPlay()
{
	Super::BeginPlay();

	SetActorLocation(FVector::ZeroVector);
	SpringArm->SetRelativeRotation(FRotator(-20.0f, 0.0f, 0.0f));
}

// Called every frame
void ATetrisPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	HandleCameraRotation();
}

void ATetrisPawn::HandleCameraRotation()
{
	CurrentCameraMovementTimeDelay -= GetWorld()->DeltaTimeSeconds;

	if (bChunksMovingMode)
		return;

	//Rotating up and down

	FRotator DeltaRotator = FRotator(fVerticalAxis, 0, 0);
	DeltaRotator *= fCameraSpeed;
	FRotator CombinedRotators = DeltaRotator + SpringArm->GetRelativeRotation();
	FRotator FixedRotator = FRotator(UKismetMathLibrary::ClampAngle(CombinedRotators.Pitch, -75, 0), SpringArm->GetRelativeRotation().Yaw, 0);

	//Rotating left and right

	if (CurrentCameraMovementTimeDelay < 0.0f)
	{
		
		if (UKismetMathLibrary::Abs(fHorizontalAxis) > CameraMovementDeadzone)
		{
			DesiredCameraYaw += (90.0f * (fHorizontalAxis > 0.0f ? -1.0f : 1.0f));
			CurrentCameraMovementTimeDelay = CameraMovementTimeDelay;
		}
		 
	}
	FixedRotator.Yaw = DesiredCameraYaw;
	SpringArm->SetRelativeRotation(FixedRotator);
}

