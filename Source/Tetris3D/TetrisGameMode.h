// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "TetrisGameInstance.h"
#include "PlayArena.h"
#include "TimerManager.h"
#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TetrisGameMode.generated.h"

/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTetrisTickDelegate, float, TickTime);

UCLASS()
class TETRIS3D_API ATetrisGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ATetrisGameMode();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;


	FTimerHandle TetrisTickHandle;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float TetrisTickDelay;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		bool GameActive;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		float SpeedUpValue;

	UPROPERTY()
		float CurrentTimeSpeed;

	UPROPERTY()
		float CacheTetrisTick;

	UPROPERTY()
		bool bPostTetrisTickDone;

	UPROPERTY()
		float CachePostTetrisTick;

	UPROPERTY()
		UTetrisGameInstance* TetrisGameInstanceRef;

	UFUNCTION(BlueprintCallable, BlueprintPure)
		UTetrisGameInstance* GetTetrisGameInstance();

	UFUNCTION()
		void TetrisTick();

	UFUNCTION()
		void PostTetrisTick();
	
	UFUNCTION(BlueprintImplementableEvent)
		void SpawnNextGroupOfChunks();

	UFUNCTION()
		void RemoveLevelOfChunks(int32 Level);

	UFUNCTION()
		bool LookForDetachableChunks();

	UFUNCTION()
		int32 RemoveRemovableLevels();

	UFUNCTION(BlueprintCallable)
		void GameOver();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void ShowSummary();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void StartGame();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void SaveProgress();

	UPROPERTY()
		int32 Points;

	UPROPERTY()
		int32 StarsReached;

	UFUNCTION()
		void AddPointsByRemovedLevels(int32 RemovedLevels);

	UFUNCTION()
		void UpdateTime(float DeltaTime);

public:

	UFUNCTION(BlueprintCallable, BlueprintPure)
		int32 GetPoints() { return Points; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
		int32 GetStarsReached() { return StarsReached; }

	UPROPERTY()
		APlayArena* CurrentPlayArena;

	UPROPERTY(BlueprintAssignable)
		FTetrisTickDelegate OnTetrisTick;

	UPROPERTY(BlueprintReadOnly)
		float TimeElapsed;

	UFUNCTION()
		FORCEINLINE	bool IsPostTetrisTickDone() { return bPostTetrisTickDone; }

	UFUNCTION(BlueprintCallable)
		void GoToMainMenu();

	UFUNCTION()
		void DoManualTick();

	UFUNCTION()
		void UpdateRating();

};
