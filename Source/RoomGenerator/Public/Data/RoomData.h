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
	// Floor Mesh
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Room Meshes|Floor")
	UStaticMesh* FloorMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Room Meshes|Floor")
	FVector FloorMeshSize = FVector(400.0f, 400.0f, 100.0f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Room Meshes|Floor")
	FTransform FloorTransform;
	

};