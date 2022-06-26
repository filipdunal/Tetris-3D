// Filip Dunal 2022

#pragma once

#include "Components/BoxComponent.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PlayArena.generated.h"

UCLASS()
class TETRIS3D_API APlayArena : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APlayArena();

	UFUNCTION()
		bool IsPointInside(FVector Point) const;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UStaticMeshComponent* Floor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UStaticMeshComponent* Wall_1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UStaticMeshComponent* Wall_2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UStaticMeshComponent* Wall_3;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UStaticMeshComponent* Wall_4;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UBoxComponent* ArenaBounds;

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
		USceneComponent* SpawnPoint;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		int32 Width;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		int32 Depth;

	UFUNCTION(BlueprintNativeEvent)
		void SetArena(int32 NewWidth, int32 NewDepth);
		virtual void SetArena_Implementation(int32 NewWidth, int32 NewDepth);

		UFUNCTION(BlueprintCallable)
			FORCEINLINE int32 GetWidth() { return Width; }

		UFUNCTION(BlueprintCallable)
			FORCEINLINE int32 GetDepth() { return Depth; }


};
