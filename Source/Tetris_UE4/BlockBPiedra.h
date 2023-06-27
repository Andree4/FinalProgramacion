// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Block.h"
#include "BlockBuilder.h"
#include "BlockBPiedra.generated.h"

/**
 * 
 */
UCLASS()
class TETRIS_UE4_API ABlockBPiedra : public ABlock, public IBlockBuilder
{
	GENERATED_BODY()
public:
	ABlockBPiedra();
	FString Material;
	virtual void SetMaterial(FString myMaterial);
};
