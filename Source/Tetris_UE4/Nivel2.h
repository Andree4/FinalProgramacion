// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InterfaceNivel.h"
#include "Nivel2.generated.h"

UCLASS()
class TETRIS_UE4_API ANivel2 : public AActor,public IInterfaceNivel
{
	GENERATED_BODY()
public:
	UPROPERTY()
		class ACambiarNiv* Nivel;

	UMaterialInterface* matTab;
public:
	// Sets default values for this actor's properties
	ANivel2();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void CambiarNivel(class ACambiarNiv* _Nivel);
	virtual void SetNivel(class ACambiarNiv* _Nivel);

};
