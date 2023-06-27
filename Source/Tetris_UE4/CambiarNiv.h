// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InterfaceNivel.h"
#include "CambiarNiv.generated.h"

UCLASS()
class TETRIS_UE4_API ACambiarNiv : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACambiarNiv();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
public:
	IInterfaceNivel* Nivel1;
	IInterfaceNivel* Nivel2;
	IInterfaceNivel* Estado;
public:	
	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* TableroMesh;

	UMaterialInterface* matTab;
	ACambiarNiv* inicio();
	void CambiarNivel(ACambiarNiv* _Nivel) ;
	void CambiarNEstado(IInterfaceNivel* _Estado);
	IInterfaceNivel* getNivel1();
	IInterfaceNivel* getNivel2();
	IInterfaceNivel* getEstado();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
