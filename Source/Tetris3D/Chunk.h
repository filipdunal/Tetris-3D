// Filip Dunal 2022

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Chunk.generated.h"

UCLASS()
class TETRIS3D_API AChunk : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AChunk();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UStaticMeshComponent* VisualMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UStaticMeshComponent* GhostVisualMesh;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		bool bFloating;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float MoveLerp;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float RotateLerp;

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void UpdateMeshTransform(float TickTime);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
		bool UpdateFloatingState();

	UFUNCTION()
		FORCEINLINE	bool GetFloatingStateCache() { return bFloating; }

	UFUNCTION()
		bool GetMoveAbility(FVector Direction);

	UFUNCTION(BlueprintImplementableEvent)
		void DestroyChunk();

	UPROPERTY()
		bool IsPreviewMode;

};
