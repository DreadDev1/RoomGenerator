#include "Rooms/MasterRoom.h"

#include "Components/ArrowComponent.h"
#include "Components/HierarchicalInstancedStaticMeshComponent.h"
#include "Data/RoomData.h"

AMasterRoom::AMasterRoom()
{
	PrimaryActorTick.bCanEverTick = false;

	// Create the Hierarchical Instanced Static Mesh Component for floors
	FloorMeshComponent = CreateDefaultSubobject<UHierarchicalInstancedStaticMeshComponent>(TEXT("FloorMeshComponent"));
	RootComponent = FloorMeshComponent;
}

void AMasterRoom::BeginPlay()
{
	Super::BeginPlay();
	GenerateRoom();
	GenerateRoomMixedSizes();
}

void AMasterRoom::ClearRoom()
{
	if (FloorMeshComponent)
	{
		FloorMeshComponent->ClearInstances();
		UE_LOG(LogTemp, Log, TEXT("Room cleared"));
	}
}

void AMasterRoom::GenerateRoom()
{
	if (!RoomData)
	{
		UE_LOG(LogTemp, Error, TEXT("MasterRoom: RoomData is null! Please assign a RoomData asset."));
		return;
	}

	if (RoomData->FloorMeshes_Large.Num() == 0)
	{
		UE_LOG(LogTemp, Error, TEXT("MasterRoom: No FloorMeshes_Large in RoomData!"));
		return;
	}

	// Clear any existing floor instances
	FloorMeshComponent->ClearInstances();

	// Generate random room size
	int32 RoomSizeX = FMath::RandRange(MinRoomSizeX, MaxRoomSizeX);
	int32 RoomSizeY = FMath::RandRange(MinRoomSizeY, MaxRoomSizeY);

	UE_LOG(LogTemp, Log, TEXT("Generating uniform room with size: %d x %d tiles"), RoomSizeX, RoomSizeY);

	// Get the floor tile size (using Large tiles for uniform generation)
	FVector FloorTileSize = RoomData->FloorTileSize_Large;

	// Generate uniform floor with large tiles
	for (int32 Y = 0; Y < RoomSizeY; ++Y)
	{
		for (int32 X = 0; X < RoomSizeX; ++X)
		{
			// Calculate position based on tile size
			FVector TilePosition = FVector(
				X * FloorTileSize.X,
				Y * FloorTileSize.Y,
				0.0f
			);

			// Create transform for this floor tile
			FTransform TileTransform = RoomData->FloorTransform;
			TileTransform.SetLocation(TileTransform.GetLocation() + TilePosition);

			// Randomly select a mesh from Large tiles
			UStaticMesh* SelectedMesh = RoomData->FloorMeshes_Large[FMath::RandRange(0, RoomData->FloorMeshes_Large.Num() - 1)];

			// Add instance to the mesh component
			FloorMeshComponent->AddInstanceWorldSpace(TileTransform);
		}
	}

	UE_LOG(LogTemp, Log, TEXT("Generated %d floor tiles"), RoomSizeX * RoomSizeY);
}

void AMasterRoom::GenerateRoomMixedSizes()
{
	if (!RoomData)
	{
		UE_LOG(LogTemp, Error, TEXT("MasterRoom: RoomData is null! Please assign a RoomData asset."));
		return;
	}

	// Validate we have at least some meshes
	if (RoomData->FloorMeshes_Large.Num() == 0 && 
		RoomData->FloorMeshes_Medium.Num() == 0 && 
		RoomData->FloorMeshes_Small.Num() == 0)
	{
		UE_LOG(LogTemp, Error, TEXT("MasterRoom: No floor meshes in RoomData!"));
		return;
	}

	// Clear any existing floor instances
	FloorMeshComponent->ClearInstances();

	UE_LOG(LogTemp, Log, TEXT("Generating mixed size room: %.0f x %.0f units"), RoomSize.X, RoomSize.Y);

	// Simple grid-based approach: Fill room with tiles
	// We'll use a greedy algorithm - place largest tiles first, fill gaps with smaller ones
	
	float CurrentY = 0.0f;
	int32 TileCount = 0;

	while (CurrentY < RoomSize.Y)
	{
		float CurrentX = 0.0f;
		
		while (CurrentX < RoomSize.X)
		{
			// Determine remaining space
			float RemainingX = RoomSize.X - CurrentX;
			float RemainingY = RoomSize.Y - CurrentY;

			// Try to place a tile based on available space
			FVector TileSize;
			UStaticMesh* SelectedMesh = nullptr;

			// Try Large tiles first (400x400)
			if (RemainingX >= RoomData->FloorTileSize_Large.X && 
				RemainingY >= RoomData->FloorTileSize_Large.Y &&
				RoomData->FloorMeshes_Large.Num() > 0)
			{
				TileSize = RoomData->FloorTileSize_Large;
				SelectedMesh = RoomData->FloorMeshes_Large[FMath::RandRange(0, RoomData->FloorMeshes_Large.Num() - 1)];
			}
			// Try Medium tiles (400x200 or smaller)
			else if (RemainingX >= RoomData->FloorTileSize_Medium.X && 
					 RemainingY >= RoomData->FloorTileSize_Medium.Y &&
					 RoomData->FloorMeshes_Medium.Num() > 0)
			{
				TileSize = RoomData->FloorTileSize_Medium;
				SelectedMesh = RoomData->FloorMeshes_Medium[FMath::RandRange(0, RoomData->FloorMeshes_Medium.Num() - 1)];
			}
			// Try Small tiles (200x200 or smaller)
			else if (RemainingX >= RoomData->FloorTileSize_Small.X && 
					 RemainingY >= RoomData->FloorTileSize_Small.Y &&
					 RoomData->FloorMeshes_Small.Num() > 0)
			{
				TileSize = RoomData->FloorTileSize_Small;
				SelectedMesh = RoomData->FloorMeshes_Small[FMath::RandRange(0, RoomData->FloorMeshes_Small.Num() - 1)];
			}
			else
			{
				// Can't fit any tile, move to next row
				break;
			}

			if (SelectedMesh)
			{
				// Create transform for this floor tile
				FVector TilePosition = FVector(CurrentX, CurrentY, 0.0f);
				FTransform TileTransform = RoomData->FloorTransform;
				TileTransform.SetLocation(TileTransform.GetLocation() + TilePosition);

				// Add instance to the mesh component
				FloorMeshComponent->AddInstanceWorldSpace(TileTransform);
				TileCount++;

				// Move to next position
				CurrentX += TileSize.X;
			}
			else
			{
				break;
			}
		}

		// Move to next row (use the smallest tile height to advance)
		if (RoomData->FloorMeshes_Small.Num() > 0)
		{
			CurrentY += RoomData->FloorTileSize_Small.Y;
		}
		else if (RoomData->FloorMeshes_Medium.Num() > 0)
		{
			CurrentY += RoomData->FloorTileSize_Medium.Y;
		}
		else if (RoomData->FloorMeshes_Large.Num() > 0)
		{
			CurrentY += RoomData->FloorTileSize_Large.Y;
		}
		else
		{
			break;
		}
	}

	UE_LOG(LogTemp, Log, TEXT("Generated %d mixed size floor tiles"), TileCount);
}