// Fill out your copyright notice in the Description page of Project Settings.

#include "TetrisGameInstance.h"
#include "Math/UnrealMathUtility.h"
#include "GroupOfChunks.h"
#include "Chunk.h"
#include "TetrisGameMode.h"
#include <Runtime/Engine/Classes/Kismet/GameplayStatics.h>

ATetrisGameMode::ATetrisGameMode()
{
	PrimaryActorTick.bCanEverTick = true;
	TetrisTickDelay = 1.f;
	CurrentTimeSpeed = 1.f;
}

void ATetrisGameMode::BeginPlay()
{
	Super::BeginPlay();
}

void ATetrisGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!GameActive)
		return;

	UpdateTime(DeltaTime);
	UpdateRating();

	CacheTetrisTick -= (DeltaTime * CurrentTimeSpeed);
	if (CacheTetrisTick < 0)
	{
		CacheTetrisTick = TetrisTickDelay;
		TetrisTick();
		bPostTetrisTickDone = false;
	}

	if (!bPostTetrisTickDone && CacheTetrisTick < (TetrisTickDelay - (FMath::Min(TetrisTickDelay * 0.1f, 0.1f))))
	{
		PostTetrisTick();
		bPostTetrisTickDone = true;
	}
}

void ATetrisGameMode::DoManualTick()
{
	CacheTetrisTick = TetrisTickDelay;
	PostTetrisTick();
	bPostTetrisTickDone = true;
}

void ATetrisGameMode::UpdateRating()
{
	if (GetTetrisGameInstance()->CurrentLevel.ThreeStarsRequirement <= Points)
		StarsReached = 3;
	else if ((GetTetrisGameInstance()->CurrentLevel.TwoStarsRequirement <= Points))
		StarsReached = 2;
	else if ((GetTetrisGameInstance()->CurrentLevel.OneStarRequirement <= Points))
		StarsReached = 1;
	else
		StarsReached = 0;
}

UTetrisGameInstance* ATetrisGameMode::GetTetrisGameInstance()
{
	if (!IsValid(TetrisGameInstanceRef))
		TetrisGameInstanceRef = Cast<UTetrisGameInstance>(GetGameInstance());

	return TetrisGameInstanceRef;
}

void ATetrisGameMode::TetrisTick()
{
	TArray<AActor*> GroupsOfChunks;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AGroupOfChunks::StaticClass(), GroupsOfChunks);

	if (GroupsOfChunks.Num() == 0 || (GroupsOfChunks.Num() == 1 && Cast<AGroupOfChunks>(GroupsOfChunks[0])->IsPreviewMode))
		SpawnNextGroupOfChunks();

	for (int i = 0; i < GroupsOfChunks.Num(); i++)
	{
		Cast<AGroupOfChunks>(GroupsOfChunks[i])->ChunkTick();
	}

	OnTetrisTick.Broadcast(TetrisTickDelay);
}

void ATetrisGameMode::PostTetrisTick()
{
	bool DetachedNow = LookForDetachableChunks();
	if (DetachedNow)
	{
		int32 RemovedLevels = RemoveRemovableLevels();
		AddPointsByRemovedLevels(RemovedLevels);
		UE_LOG(LogTemp, Warning, TEXT("Removed levels num: %d"), (int)RemovedLevels);
	}

}

void ATetrisGameMode::AddPointsByRemovedLevels(int32 RemovedLevels)
{
	int32 RawPoints = RemovedLevels * CurrentPlayArena->Width * CurrentPlayArena->Depth;

	Points += (RawPoints);
}

void ATetrisGameMode::UpdateTime(float DeltaTime)
{
	TimeElapsed += DeltaTime;

	if (GetTetrisGameInstance()->CurrentLevel.TargetTime != 0) //Is time attack mode
	{
		if (GetTetrisGameInstance()->CurrentLevel.TargetTime - (int32)TimeElapsed <= 0)
		{
			GameOver();
		}
	}
}

void ATetrisGameMode::RemoveLevelOfChunks(int32 Level)
{
	FString TheFloatStr = "Remove level: ";
	TheFloatStr.AppendInt(Level);
	GEngine->AddOnScreenDebugMessage(-1, 7.0, FColor::Red, *TheFloatStr);

	TArray<AActor*> AllChunksActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AChunk::StaticClass(), AllChunksActors);
	for (auto& Elem : AllChunksActors)
	{
		if (Cast<AChunk>(Elem)->IsPreviewMode)
			continue;

		if (FMath::IsNearlyEqual(Elem->GetActorLocation().Z, Level, 0.5f))
		{
			Cast<AChunk>(Elem)->DestroyChunk();
		}
		else if(Elem->GetActorLocation().Z > Level + 0.5f)
		{
			Elem->AddActorWorldOffset(FVector(0, 0, -100));
		}
	}
}

bool ATetrisGameMode::LookForDetachableChunks()
{
	bool Detached = false;

	TArray<AActor*> GroupsOfChunks;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AGroupOfChunks::StaticClass(), GroupsOfChunks);
	for (int i = 0; i < GroupsOfChunks.Num(); i++)
	{
		if (Cast<AGroupOfChunks>(GroupsOfChunks[i])->CheckForNeedToDetach())
			Detached = true;
	}

	return Detached;
}

int32 ATetrisGameMode::RemoveRemovableLevels()
{
	TArray<AActor*> AllChunksActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AChunk::StaticClass(), AllChunksActors);

	FString TheFloatStr = "Chunks: ";
	TheFloatStr.AppendInt(AllChunksActors.Num());
	GEngine->AddOnScreenDebugMessage(-1, 7.0, FColor::Red, *TheFloatStr);

	for (int i = 0; i <= 20; i++)
	{
		int CoordZ = 50 + (i * 100);

		int ChunksNum = 0;
		for (auto& Elem : AllChunksActors)
		{
			if (Cast<AChunk>(Elem)->IsPreviewMode)
				continue; 
			if (FMath::IsNearlyEqual(Elem->GetActorLocation().Z, CoordZ, 0.5f))
				ChunksNum++;

		}
		if (ChunksNum == (CurrentPlayArena->GetWidth() * CurrentPlayArena->GetDepth()))
		{
			RemoveLevelOfChunks(CoordZ);
			return 1 + RemoveRemovableLevels();
		}
	}
	return 0;
}

void ATetrisGameMode::GoToMainMenu()
{
	UGameplayStatics::OpenLevel(this, FName(*GetWorld()->GetName()), false);
}

void ATetrisGameMode::GameOver()
{
	GameActive = false;
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->SetInputMode(FInputModeUIOnly());
	ShowSummary();
	SaveProgress();
}


