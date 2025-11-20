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

	if (!RoomData->FloorMesh)
	{
		UE_LOG(LogTemp, Error, TEXT("MasterRoom: FloorMesh is null in RoomData!"));
		return;
	}

	// Clear any existing floor instances
	FloorMeshComponent->ClearInstances();

	// Set the static mesh for the floor component
	FloorMeshComponent->SetStaticMesh(RoomData->FloorMesh);

	// Generate random room size
	int32 RoomSizeX = FMath::RandRange(MinRoomSizeX, MaxRoomSizeX);
	int32 RoomSizeY = FMath::RandRange(MinRoomSizeY, MaxRoomSizeY);

	UE_LOG(LogTemp, Log, TEXT("Generating room with size: %d x %d"), RoomSizeX, RoomSizeY);

	// Generate the floor
	GenerateFloor(RoomSizeX, RoomSizeY);
}

void AMasterRoom::GenerateFloor(int32 RoomSizeX, int32 RoomSizeY)
{
	if (!RoomData)
	{
		return;
	}

	// Get the floor mesh size from RoomData
	FVector FloorTileSize = RoomData->FloorMeshSize;

	// Loop through and create floor tiles
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

			// Add instance to the mesh component
			FloorMeshComponent->AddInstance(TileTransform);
		}
	}

	UE_LOG(LogTemp, Log, TEXT("Generated %d floor tiles"), RoomSizeX * RoomSizeY);
}