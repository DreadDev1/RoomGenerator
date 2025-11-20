// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Data/RoomData.h"
#include "MasterRoom.generated.h"

UCLASS()
class ROOMGENERATOR_API AMasterRoom : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMasterRoom();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Room Data Asset Reference
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Room")
	URoomData* RoomData;

	// Room Size Configuration (in tiles)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Room Generation|Size", meta = (ClampMin = "1"))
	int32 MinRoomSizeX = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Room Generation|Size", meta = (ClampMin = "1"))
	int32 MaxRoomSizeX = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Room Generation|Size", meta = (ClampMin = "1"))
	int32 MinRoomSizeY = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Room Generation|Size", meta = (ClampMin = "1"))
	int32 MaxRoomSizeY = 1;

	// Generation functions
	UFUNCTION(BlueprintCallable, CallInEditor, Category = "Room Generation")
	void GenerateRoom();

	UFUNCTION(BlueprintCallable, CallInEditor, Category = "Room Generation")
	void ClearRoom();

protected:
	void GenerateFloor(int32 RoomSizeX, int32 RoomSizeY);

protected:
	// Instanced Mesh Component for Floor
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Room Components")
	class UHierarchicalInstancedStaticMeshComponent* FloorMeshComponent;

};