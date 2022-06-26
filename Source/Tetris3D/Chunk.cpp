// Filip Dunal 2022

#include "Math/UnrealMathVectorCommon.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "Chunk.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "TetrisGameMode.h"

// Sets default values
AChunk::AChunk()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Mesh = CreateDefaultSubobject <UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;

	VisualMesh = CreateDefaultSubobject <UStaticMeshComponent>(TEXT("Visual Mesh"));
	VisualMesh->SetupAttachment(Mesh);
	
	GhostVisualMesh = CreateDefaultSubobject <UStaticMeshComponent>(TEXT("Ghost Visual Mesh"));
	GhostVisualMesh->SetupAttachment(Mesh);
	//VisualMesh->SetUsingAbsoluteLocation(true);

	VisualMesh->SetWorldLocation(Mesh->GetComponentLocation());
	bFloating = true;
}

// Called when the game starts or when spawned
void AChunk::BeginPlay()
{
	Super::BeginPlay();

	Mesh->SetVisibility(false);
	VisualMesh->SetVisibility(true);
	VisualMesh->SetWorldLocation(Mesh->GetComponentLocation());
}

bool AChunk::UpdateFloatingState()
{
	if (!bFloating)
		return false; //Prevent from unfreezing chunks (this should never happen)

	FHitResult OutHit;
	FVector Start = GetActorLocation();
	FVector End = Start - FVector(0, 0, 100);
	FCollisionQueryParams ChunkTraceParams = FCollisionQueryParams(FName(TEXT("Chunk Fall Trace")), true, this);

	//DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 5.0f, (uint8)0U, 5.0f);

	if (GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, ECC_Visibility, ChunkTraceParams))
	{
		AChunk* HitChunk = Cast<AChunk>(OutHit.Actor);
		if (IsValid(HitChunk))
		{
			bFloating = HitChunk->bFloating;
			return bFloating;
		}
		bFloating = false;
		return bFloating;
	}
	bFloating = true;
	return true;
}

bool AChunk::GetMoveAbility(FVector Direction)
{
	if (!Direction.Normalize())
		return true; //Moving without moving is possible

	FHitResult OutHit;
	FVector Start = GetActorLocation() + FVector(0,0,50);
	FVector End = Start + (Direction * 100);
	FCollisionQueryParams ChunkTraceParams = FCollisionQueryParams(FName(TEXT("Chunk Move Trace")), true, this);

	//DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 5.0f, (uint8)0U, 1.0f);

	if (GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, ECC_WorldDynamic, ChunkTraceParams))
	{
		AChunk* HitChunk = Cast<AChunk>(OutHit.Actor);
		if (IsValid(HitChunk))
			return HitChunk->GetFloatingStateCache();
		else
			return false;
	}

	return true;
}

// Called every frame
void AChunk::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

