// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "RoomData.generated.h"

/**
 * 
 */
UCLASS()
class ROOMGENERATOR_API URoomData : public UDataAsset
{
	GENERATED_BODY()
	
public:
	// Floor Meshes - Categorized by Size
	// Large tiles (400x400)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Room Meshes|Floor|Large")
	TArray<UStaticMesh*> FloorMeshes_Large;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Room Meshes|Floor|Large")
	FVector FloorTileSize_Large = FVector(400.0f, 400.0f, 25.0f);

	// Medium tiles (400x200, 200x400, etc.)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Room Meshes|Floor|Medium")
	TArray<UStaticMesh*> FloorMeshes_Medium;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Room Meshes|Floor|Medium")
	FVector FloorTileSize_Medium = FVector(400.0f, 200.0f, 25.0f);

	// Small tiles (200x200, 100x100, etc.)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Room Meshes|Floor|Small")
	TArray<UStaticMesh*> FloorMeshes_Small;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Room Meshes|Floor|Small")
	FVector FloorTileSize_Small = FVector(200.0f, 200.0f, 25.0f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Room Meshes|Floor")
	FTransform FloorTransform;

	// Wall Meshes
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Room Meshes|Walls")
	UStaticMesh* WallBottomMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Room Meshes|Walls")
	FVector WallMeshSize = FVector(400.0f, 100.0f, 100.0f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Room Meshes|Walls")
	FTransform WallTransform;
	

};