// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "Block.h"
#include "BlockBAgua.h"
#include "BlockBBrillante.h"
#include "BlockBMadera.h"
#include "BlockBPiedra.h"
#include <functional>
#include "Pieces.generated.h"

UCLASS()
class TETRIS_UE4_API APieces : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APieces();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
    
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

    UPROPERTY()
    class USceneComponent* SceneComponent;
    
    UPROPERTY()
    class USoundCue* RotateSoundCue;
    
    UPROPERTY()
    class USoundCue* MoveLeftRightSoundCue;
    
    UPROPERTY()
    TArray<class UMaterial*> Colors;
    
    void DrawDebugLines();
    
    void TestRotate();
    void MoveLeft();
    void MoveRight();
    bool MoveDown(bool PlaySound=true);
    void Dismiss();
    bool CheckWillCollision(std::function<FVector(FVector OldLocation)> ChangeBeforeCheck);
    void MoverPieza(FVector NewPosition);
    void NumA();
    int numa;
    void setIndex(int _numa) { numa = _numa; }
    int getIndex() { return numa; }
    void SpawnBlocks();
private:
    TArray<ABlock*> Blocks;
    
   
    
public:
    struct FConstructorStatics
    {
        ConstructorHelpers::FObjectFinderOptional<UMaterial> Color_0;
        ConstructorHelpers::FObjectFinderOptional<UMaterial> Color_1;
        ConstructorHelpers::FObjectFinderOptional<UMaterial> Color_2;
        ConstructorHelpers::FObjectFinderOptional<UMaterial> Color_3;
        ConstructorHelpers::FObjectFinderOptional<UMaterial> Color_4;
        ConstructorHelpers::FObjectFinderOptional<UMaterial> Color_5;
        ConstructorHelpers::FObjectFinderOptional<UMaterial> Color_6;
        FConstructorStatics()
            : Color_0(TEXT("Material'/Game/StarterContent/Materials/M_Wood_Oak.M_Wood_Oak'"))
            , Color_1(TEXT("Material'/Game/StarterContent/Materials/M_Water_Lake.M_Water_Lake'"))
            , Color_2(TEXT("Material'/Game/StarterContent/Materials/M_Tech_Hex_Tile_Pulse.M_Tech_Hex_Tile_Pulse'"))
            , Color_3(TEXT("Material'/Game/StarterContent/Materials/M_CobbleStone_Rough.M_CobbleStone_Rough'"))
            , Color_4(TEXT("Material'/Game/Mesh/Material_4.Material_4'"))
            , Color_5(TEXT("Material'/Game/Mesh/Material_5.Material_5'"))
            , Color_6(TEXT("Material'/Game/Mesh/Material_6.Material_6'"))
        {
        }
    };

};
