// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Pieces.h"
#include "CambiarNiv.h"
#include "Board.generated.h"

UCLASS()
class TETRIS_UE4_API ABoard : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ABoard();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    void Rotate(); // 旋转90度，如果不能旋转则忽略
    void MoveLeft();  // 向左移动一格
    void MoveRight(); // 向右移动一格
    void MoveDown(); // 向下移动一格
    void NewPieces(); // 随机产生一个新的形状
    void CheckLine(); // 检查是否某一行可以消除了
    void CheckLine2();
    void MoveDownToEnd(); // 向下移动到不能移动为止
    void FinJuego();
    void Reinicio();
    void MoverPiezaa();
    void CambiarNivel();
  
public:
    int band =0;
    int NPiezas=3, NBlockD=0,NLineas =0 ;
    float Puntaje;

    UPROPERTY()
    APieces* CurrentPieces;
    APieces* SigPiece;
    APieces* SubSPiece;

    UPROPERTY(EditAnywhere)
        UStaticMeshComponent* MeshEscenario;
    UPROPERTY()
    class USoundCue* LineRemoveSoundCue;
    
    UPROPERTY()
    class USoundCue* MoveToEndSoundCue;
    
    UPROPERTY()
    class USoundCue* GameOverSoundCue;
    
    UPROPERTY()
    class USoundCue* GameStartSoundCue;
    UPROPERTY()
        ACambiarNiv* Nivel;

    UPROPERTY()
        ACambiarNiv* Niv;
private:
    enum PiecesStatus { PS_NOT_INITED, PS_MOVING, PS_GOT_BOTTOM};
    PiecesStatus Status = PS_NOT_INITED;
    
    const float CoolDown = 0.5f;
    float CoolLeft = 0.5f;
    bool bGameOver = false;
    
    bool CheckGameOver();
};
