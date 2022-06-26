// Filip Dunal 2022

#include "GroupOfChunks.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "Chunk.h"
#include "TetrisPlayerController.h"
#include "Kismet/GameplayStatics.h"

void ATetrisPlayerController::BeginPlay()
{
	Super::BeginPlay();

	TetrisPawn = Cast<ATetrisPawn>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	TetrisGameMode = Cast<ATetrisGameMode>(GetWorld()->GetAuthGameMode());

	FInputAxisBinding X = InputComponent->BindAxis("LookVertical", this, &ATetrisPlayerController::ReadVerticalAxis);
	X.bConsumeInput = false;

	InputComponent->BindAxis("LookHorizontal", this, &ATetrisPlayerController::ReadHorizontalAxis);
	InputComponent->BindAxis("LookVertical", TetrisPawn, &ATetrisPawn::ReadVerticalAxis);
	InputComponent->BindAxis("LookHorizontal", TetrisPawn, &ATetrisPawn::ReadHorizontalAxis);

	InputComponent->BindAction("MovingChunksMode", IE_Pressed, this, &ATetrisPlayerController::EnableChunksMoving);
	InputComponent->BindAction("MovingChunksMode", IE_Released, this, &ATetrisPlayerController::DisableChunksMoving);
	InputComponent->BindAction("MovingChunksMode", IE_Pressed, TetrisPawn, &ATetrisPawn::EnableCameraMoving);
	InputComponent->BindAction("MovingChunksMode", IE_Released, TetrisPawn, &ATetrisPawn::DisableCameraMoving);

	InputComponent->BindAction("RotateChunksX", IE_Pressed, this, &ATetrisPlayerController::RotateChunksX);
	InputComponent->BindAction("RotateChunksY", IE_Pressed, this, &ATetrisPlayerController::RotateChunksY);
	InputComponent->BindAction("RotateChunksZ", IE_Pressed, this, &ATetrisPlayerController::RotateChunksZ);

	//InputComponent->BindAction("SpeedUp", IE_Pressed, TetrisGameMode, &ATetrisGameMode::MakeTimeFaster);
	//InputComponent->BindAction("SpeedUp", IE_Released, TetrisGameMode, &ATetrisGameMode::MakeTimeNormal);
	InputComponent->BindAction("SpeedUp", IE_Pressed, this, &ATetrisPlayerController::LandGroupOfChunks);

	bChunksMovingMode = true;

}

void ATetrisPlayerController::EnableChunksMoving()
{
	bChunksMovingMode = false;
}

void ATetrisPlayerController::DisableChunksMoving()
{
	bChunksMovingMode = true;
}

void ATetrisPlayerController::Tick(float DeltaTime)
{
	UpdateCameraDirection();
	HandleChunksMoving();

	//FString TheFloatStr = FString::SanitizeFloat(MovementStorage.X) + " X " + FString::SanitizeFloat(MovementStorage.Y);
	//GEngine->AddOnScreenDebugMessage(-1, 1.0, FColor::Red, *TheFloatStr);
}

void ATetrisPlayerController::UpdateCameraDirection()
{
	float Yaw = TetrisPawn->GetActorRotation().Yaw;
	if (Yaw >= -45 && Yaw < 45)
		CamDirection = CameraDirection::SOUTH;
	else if (Yaw >= 45 && Yaw < 135)
		CamDirection = CameraDirection::WEST;
	else if (Yaw >= 135 && Yaw < 225)
		CamDirection = CameraDirection::NORTH;
	else
		CamDirection = CameraDirection::EAST;
}

void ATetrisPlayerController::HandleChunksMoving()
{
	CurrentMovementTimeDelay -= GetWorld()->DeltaTimeSeconds;

	if (!bChunksMovingMode)
		return;

	FVector Move = GetDesiredMove();

	if (CurrentMovementTimeDelay < 0.0f)// && TetrisGameMode->IsPostTetrisTickDone())
	{
		AGroupOfChunks* GroupOfChunks = GetGroupOfChunks();
		if (!IsValid(GroupOfChunks))
			return;

		if (IsMovePossible(Move, GroupOfChunks))
		{
			GroupOfChunks->AddActorWorldOffset(Move);
			CurrentMovementTimeDelay = MovementTimeDelay;
			TetrisGameMode->OnTetrisTick.Broadcast(0.f);
		}
	}
}

FVector ATetrisPlayerController::GetDesiredMove()
{
	float MoveVertical = fVerticalAxis > 0.5 ? 1 : fVerticalAxis < -0.5 ? -1 : 0;
	float MoveHorizontal = fHorizontalAxis > 0.5 ? 1 : fHorizontalAxis < -0.5 ? -1 : 0;

	//Prevent from moving diagonally 
	if (FMath::Abs(MoveVertical) == FMath::Abs(MoveVertical) == 1)
	{
		if (FMath::Abs(fVerticalAxis) > FMath::Abs(fHorizontalAxis))
			MoveHorizontal = 0;
		else
			MoveVertical = 0;
	}

	FVector Move = FVector(MoveVertical, MoveHorizontal, 0) * 100;
	switch (CamDirection)
	{
	case CameraDirection::WEST:
		Move = FVector(-Move.Y, Move.X, 0);
		break;
	case CameraDirection::NORTH:
		Move *= -1;
		break;
	case CameraDirection::EAST:
		Move = FVector(Move.Y, -Move.X, 0);
		break;
	}

	//FString TheFloatStr = FString::SanitizeFloat(Move.X) + " X " + FString::SanitizeFloat(Move.Y);
	//GEngine->AddOnScreenDebugMessage(-1, 1.0, FColor::Red, *TheFloatStr);

	return Move;
}

AGroupOfChunks* ATetrisPlayerController::GetGroupOfChunks()
{
	//TArray<AGroupOfChunks*> GroupsOfChunks;
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AGroupOfChunks::StaticClass(), FoundActors);

	if (FoundActors.Num() == 0)
		return nullptr;

	for (AActor* GroupOfChunks : FoundActors)
	{
		AGroupOfChunks* Result = Cast<AGroupOfChunks>(GroupOfChunks);
		if (!Result->IsPreviewMode)
			return Result;
	}
	return nullptr;
}


bool ATetrisPlayerController::IsMovePossible(FVector Direction, AGroupOfChunks* GroupOfChunks)
{
	TArray<UActorComponent*> Comps;
	Comps = GroupOfChunks->GetComponentsByClass(UChildActorComponent::StaticClass());

	for (int i = 0; i < Comps.Num(); i++)
	{
		UChildActorComponent* ChildActorComp = Cast<UChildActorComponent>(Comps[i]);
		AActor* TempActor = ChildActorComp->GetChildActor();
		AChunk* TempChunk = Cast<AChunk>(TempActor);

		if (!TempChunk->GetMoveAbility(Direction))
			return false;
	}
	return true;
}

void ATetrisPlayerController::RotateChunksX()
{
	if (!TetrisGameMode->IsPostTetrisTickDone())
		return;

	AGroupOfChunks* GroupOfChunks = GetGroupOfChunks();
	if (!IsValid(GroupOfChunks))
		return;
	GroupOfChunks->Rotate(FRotator(90,0,0));
}

void ATetrisPlayerController::RotateChunksY()
{
	if (!TetrisGameMode->IsPostTetrisTickDone())
		return;

	AGroupOfChunks* GroupOfChunks = GetGroupOfChunks();
	if (!IsValid(GroupOfChunks))
		return;
	GroupOfChunks->Rotate(FRotator(0, 90, 0));
}

void ATetrisPlayerController::RotateChunksZ()
{
	if (!TetrisGameMode->IsPostTetrisTickDone())
		return;

	AGroupOfChunks* GroupOfChunks = GetGroupOfChunks();
	if (!IsValid(GroupOfChunks))
		return;
	GroupOfChunks->Rotate(FRotator(0, 0, 90));
}

void ATetrisPlayerController::LandGroupOfChunks()
{
	AGroupOfChunks* GroupOfChunks = GetGroupOfChunks();
	if (!IsValid(GroupOfChunks))
		return;

	GroupOfChunks->FallNow();
}

