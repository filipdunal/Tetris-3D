// Filip Dunal 2022

#include "TetrisGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "PlayArena.h"

// Sets default values
APlayArena::APlayArena()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultRoot"));
	Floor = CreateDefaultSubobject <UStaticMeshComponent>(TEXT("Floor"));
	
	Wall_1 = CreateDefaultSubobject <UStaticMeshComponent>(TEXT("Wall_1"));
	Wall_1->SetupAttachment(RootComponent);
	Wall_2 = CreateDefaultSubobject <UStaticMeshComponent>(TEXT("Wall_2"));
	Wall_2->SetupAttachment(RootComponent);
	Wall_3 = CreateDefaultSubobject <UStaticMeshComponent>(TEXT("Wall_3"));
	Wall_3->SetupAttachment(RootComponent);
	Wall_4 = CreateDefaultSubobject <UStaticMeshComponent>(TEXT("Wall_4"));
	Wall_4->SetupAttachment(RootComponent);
	ArenaBounds = CreateDefaultSubobject <UBoxComponent>(TEXT("Arena bounds"));
	ArenaBounds->SetupAttachment(RootComponent);
	SpawnPoint = CreateDefaultSubobject <USceneComponent>(TEXT("Spawn point"));
	SpawnPoint->SetupAttachment(RootComponent);
}

bool APlayArena::IsPointInside(FVector Point) const
{
	FVector BoxExtent = ArenaBounds->GetScaledBoxExtent();

	if (Point.X > BoxExtent.X || Point.X < -BoxExtent.X)
		return false;

	if (Point.Y > BoxExtent.Y || Point.Y < -BoxExtent.Y)
		return false;

	if (Point.Z < 0 || Point.Z > BoxExtent.Z * 2)
		return false;

	return true;
}

// Called when the game starts or when spawned
void APlayArena::BeginPlay()
{
	Super::BeginPlay();
	AGameModeBase* GameMode = UGameplayStatics::GetGameMode(GetWorld());
	ATetrisGameMode* TetrisGameMode = Cast<ATetrisGameMode>(GameMode);
	if (IsValid(TetrisGameMode))
	{
		TetrisGameMode->CurrentPlayArena = this;
	}
}

// Called every frame
void APlayArena::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APlayArena::SetArena_Implementation(int32 NewWidth, int32 NewDepth)
{
}

