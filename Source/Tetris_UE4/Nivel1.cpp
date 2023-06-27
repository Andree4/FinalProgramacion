// Fill out your copyright notice in the Description page of Project Settings.


#include "Nivel1.h"
#include "CambiarNiv.h"

// Sets default values
ANivel1::ANivel1()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	matTab = LoadObject<UMaterialInterface>(nullptr, TEXT("Material'/Game/Mesh/Material_6.Material_6'"));
}

// Called when the game starts or when spawned
void ANivel1::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ANivel1::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ANivel1::CambiarNivel(ACambiarNiv* _Nivel)
{
	_Nivel->TableroMesh->SetMaterial(0, matTab);
	_Nivel->TableroMesh->SetWorldScale3D(FVector(1.0f, 2.0f, 1.0f));
	Nivel->CambiarNEstado(Nivel->getNivel2());
}

void ANivel1::SetNivel(ACambiarNiv* _Nivel)
{
	Nivel = _Nivel;
}

