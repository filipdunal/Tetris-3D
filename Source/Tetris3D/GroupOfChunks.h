// Filip Dunal 2022

#pragma once

#include "TetrisGameMode.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GroupOfChunks.generated.h"

UCLASS()
class TETRIS3D_API AGroupOfChunks : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGroupOfChunks();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
		void ChunkTick();

	UFUNCTION(BlueprintNativeEvent)
		void DetachChunks();
		virtual void DetachChunks_Implementation();

	UFUNCTION()
		bool CheckForNeedToDetach();

	UFUNCTION()
		void Rotate(FRotator Rotator);

	UFUNCTION()
		bool CanRotate(FRotator Rotator, TMap<AChunk*,FVector> &OutLocations);

	UFUNCTION()
		TArray<AChunk*> GetChildChunks();

	UFUNCTION()
		TArray<AActor*> GetChildChunksAsActors();

	UFUNCTION()
		void NormalizeLocation();

	UFUNCTION()
		bool ArePositionsFree(TArray<FVector> PositionsToCheck, FVector Offset);

	UFUNCTION()
		void AddOffsetsToLocations(TMap<AChunk*, FVector>& OutLocations, FVector Offset);

	UFUNCTION()
		void FallNow();

	UFUNCTION(BlueprintCallable, BlueprintPure)
		float GetFallDistance();

	UPROPERTY(BlueprintReadWrite)
		class ATetrisGameMode* GameModeRef;

	UPROPERTY()
		TArray<FVector> PossibleOffsets;

	UPROPERTY()
		bool IsPreviewMode;

	UFUNCTION(BlueprintCallable)
		void SetPreviewMode(bool value);
};
