// Filip Dunal 2022

#pragma once

#include "GroupOfChunks.h"
#include "Engine/DataTable.h"
#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "TetrisGameInstance.generated.h"


/**
 * 
 */

UENUM(BlueprintType)
enum class FGameType : uint8
{
	REACH_SCORE = 0 UMETA(DisplayName = "Reach score"),
	SCULPTURE = 1 UMETA(DisplayName = "Sculpture")
};

USTRUCT(BlueprintType)
struct FLevelStruct : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 LevelNumber;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 MapDepth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 MapWidth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float TickTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool GhostMeshVisible;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<TSubclassOf<class AGroupOfChunks>> AllowedGroupOfChunks;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FGameType GameType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 OneStarRequirement;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 TwoStarsRequirement;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 ThreeStarsRequirement;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 TargetTime;
};

USTRUCT(BlueprintType)
struct FWorldStruct : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString WorldID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FText Title;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FLevelStruct> Levels;
};

USTRUCT(BlueprintType)
struct FLevelSaveStruct : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString WorldID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 LevelNumber;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 Highscore;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 Stars;
};

UCLASS()
class TETRIS3D_API UTetrisGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:

	virtual void Init() override;
	virtual void StartGameInstance() override;
	virtual void OnStart() override;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		FLevelStruct CurrentLevel;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		FWorldStruct CurrentWorld;
};
