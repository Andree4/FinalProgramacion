// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Block.h"
#include "BlockBuilder.h"
#include "BlockBAgua.generated.h"

/**
 * 
 */
UCLASS()
class TETRIS_UE4_API ABlockBAgua : public ABlock, public IBlockBuilder
{
	GENERATED_BODY()
public:
	ABlockBAgua();
	FString Material;
	virtual void SetMaterial(FString myMaterial);
};
