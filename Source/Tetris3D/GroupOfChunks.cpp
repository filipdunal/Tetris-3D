// Filip Dunal 2022

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Chunk.h"
#include "GroupOfChunks.h"
#include "DrawDebugHelpers.h"

// Sets default values
AGroupOfChunks::AGroupOfChunks()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PossibleOffsets.Empty();
	PossibleOffsets.Add(FVector(-100.f, 0.f, 0.f));
	PossibleOffsets.Add(FVector(100.f, 0.f, 0.f));
	PossibleOffsets.Add(FVector(0.f, -100.f, 0.f));
	PossibleOffsets.Add(FVector(0.f, 100.f, 0.f));
	PossibleOffsets.Add(FVector(0.f, 0.f, -100.f));
}

// Called when the game starts or when spawned
void AGroupOfChunks::BeginPlay()
{
	Super::BeginPlay();

	AGameModeBase* GameMode = UGameplayStatics::GetGameMode(GetWorld());
	GameModeRef = Cast<ATetrisGameMode>(GameMode);

	NormalizeLocation();
}

void AGroupOfChunks::NormalizeLocation()
{
	UActorComponent* Comp = GetComponentByClass(UChildActorComponent::StaticClass());
	if (!IsValid(Comp))
		return;

	UChildActorComponent* ChildActorComp = Cast<UChildActorComponent>(Comp);
	AActor* ChildActor = ChildActorComp->GetChildActor();
	AChunk* Chunk = Cast<AChunk>(ChildActor);
	if (!IsValid(Chunk))
		return;

	FVector RelativeLoc = UKismetMathLibrary::InverseTransformLocation(GetTransform(), Chunk->GetActorLocation());
	RelativeLoc = FVector((int)RelativeLoc.X % 100, (int)RelativeLoc.Y % 100, (int)RelativeLoc.Z % 100);

	if (FMath::IsNearlyEqual(RelativeLoc.X, 0.0f, 1.0f))
		AddActorLocalOffset(FVector(50.0f, 0.0f, 0.0f));

	if (FMath::IsNearlyEqual(RelativeLoc.Y, 0.0f, 1.0f))
		AddActorLocalOffset(FVector(0.0f, 50.0f, 0.0f));

	if (FMath::IsNearlyEqual(RelativeLoc.Z, 0.0f, 1.0f))
		AddActorLocalOffset(FVector(0.0f, 0.0f, 50.0f));
}


// Called every frame
void AGroupOfChunks::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGroupOfChunks::ChunkTick()
{
	//CheckState(); // Already checked in PostTick in game mode, not necessary

	if (IsPreviewMode)
		return;
	AddActorWorldOffset(FVector(0, 0, -100));
}

bool AGroupOfChunks::CheckForNeedToDetach()
{
	if (IsPreviewMode)
		return false;

	TArray<AChunk*> ChildChunks = GetChildChunks();
	for(int i= 0; i< ChildChunks.Num(); i++)
	{
		if (ChildChunks[i]->UpdateFloatingState() == false)
		{
			DetachChunks();
			return true;
		}
	}

	return false;
}

void AGroupOfChunks::DetachChunks_Implementation()
{

}

void AGroupOfChunks::Rotate(FRotator Rotator)
{
	TMap<AChunk*, FVector> LocationsMap = TMap<AChunk*,FVector>();
	if (!CanRotate(Rotator, LocationsMap))
		return;

	for (auto& Elem : LocationsMap)
	{
		Elem.Key->SetActorLocation(Elem.Value);
		Elem.Key->AddActorLocalRotation(Rotator);
	}

	GameModeRef->OnTetrisTick.Broadcast(0.f);
}

bool AGroupOfChunks::CanRotate(FRotator Rotator, TMap<AChunk*,FVector>& OutLocations)
{
	TArray<AChunk*> ChildChunks = GetChildChunks();

	//Get desired positions
	for (AChunk* Chunk : ChildChunks)
	{
		FVector RelativeLoc = UKismetMathLibrary::InverseTransformLocation(GetTransform(), Chunk->GetActorLocation());
		FVector RotatedRelativeLoc = Rotator.RotateVector(RelativeLoc);
		FVector RotatedWorldLoc = UKismetMathLibrary::TransformLocation(GetTransform(), RotatedRelativeLoc);
		OutLocations.Add(Chunk, RotatedWorldLoc);
	}

	TArray<FVector> LocationsArray;
	OutLocations.GenerateValueArray(LocationsArray);

	//Test normal values
	if (ArePositionsFree(LocationsArray, FVector(0.0f,0.0f,0.0f)))
		return true;

	//Test with offsets
	for (auto CurrentPossibleOffset : PossibleOffsets)
	{
		if (ArePositionsFree(LocationsArray, CurrentPossibleOffset))
		{
			AddOffsetsToLocations(OutLocations, CurrentPossibleOffset);
			return true;
		}
	}
	
	return false;
}

void AGroupOfChunks::AddOffsetsToLocations(TMap<AChunk*, FVector>& OutLocations, FVector Offset)
{
	for (auto& Elem : OutLocations)
	{
		Elem.Value += Offset;
	}
}

bool AGroupOfChunks::ArePositionsFree(TArray<FVector> PositionsToCheck, FVector Offset)
{
	TArray<TEnumAsByte<EObjectTypeQuery>> traceObjectTypes;
	traceObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldDynamic));

	for (FVector IterLocation : PositionsToCheck)
	{
		IterLocation += Offset;
		TArray<AActor*> OverlappedChunks;
		UKismetSystemLibrary::SphereOverlapActors(GetWorld(), IterLocation, 1.0f, traceObjectTypes, AChunk::StaticClass(), GetChildChunksAsActors(), OverlappedChunks);
		if (OverlappedChunks.Num() > 0)
			return false;

		if (!GameModeRef->CurrentPlayArena->IsPointInside(IterLocation))
			return false;
	}

	return true;
}


TArray<AChunk*> AGroupOfChunks::GetChildChunks()
{
	TArray<AChunk*> Result; 

	TArray<UActorComponent*> Comps = GetComponentsByClass(UChildActorComponent::StaticClass());
	for (int i = 0; i < Comps.Num(); i++)
	{
		UChildActorComponent* ChildActorComp = Cast<UChildActorComponent>(Comps[i]);
		if (IsValid(ChildActorComp))
		{
			AChunk* Chunk = Cast<AChunk>(ChildActorComp->GetChildActor());
			if (IsValid(Chunk))
				Result.Add(Chunk);
		}
	}
	return Result;
}

TArray<AActor*> AGroupOfChunks::GetChildChunksAsActors()
{
	TArray<AActor*> Result;

	TArray<UActorComponent*> Comps = GetComponentsByClass(UChildActorComponent::StaticClass());
	for (int i = 0; i < Comps.Num(); i++)
	{
		UChildActorComponent* ChildActorComp = Cast<UChildActorComponent>(Comps[i]);
		if (IsValid(ChildActorComp))
		{
			Result.Add(ChildActorComp->GetChildActor());
		}
	}
	return Result;
}

void AGroupOfChunks::FallNow()
{
	float FallDistance = GetFallDistance();
	AddActorWorldOffset(FVector(0, 0, -FallDistance));
	GameModeRef->DoManualTick();
}

float AGroupOfChunks::GetFallDistance()
{
	TArray<AChunk*> ChildChunks = GetChildChunks();

	float FallDistance = 1000;

	for (AChunk* ChunkTemp : ChildChunks)
	{
		FHitResult OutHit;
		FVector Start = ChunkTemp->GetActorLocation();
		FVector End = Start - FVector(0, 0, 2000);
		FCollisionQueryParams ChunkTraceParams = FCollisionQueryParams(FName(TEXT("Chunk Fall Trace")), true, this);

		if (GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, ECC_Visibility, ChunkTraceParams))
		{
			AChunk* HitChunk = Cast<AChunk>(OutHit.Actor);
			if (!(IsValid(HitChunk) && ChildChunks.Contains(HitChunk)))
			{
				float CurrentFallDistance = Start.Z - OutHit.Location.Z - 50.f;
				FallDistance = FMath::Min(FallDistance, CurrentFallDistance);
				//DrawDebugLine(GetWorld(), Start, Start - FVector(0,0,FallDistance), FColor::Red, false, 5.0f, (uint8)0U, 5.0f);
			}	
		}
	}

	return FallDistance;
}

void AGroupOfChunks::SetPreviewMode(bool value)
{
	IsPreviewMode = value;
	TArray<AChunk*> ChildChunks = GetChildChunks();
	for (AChunk* Chunk : ChildChunks)
	{
		Chunk->IsPreviewMode = value;
	}
}


