// Fill out your copyright notice in the Description page of Project Settings.


#include "Nivel2.h"
#include "CambiarNiv.h"
// Sets default values
ANivel2::ANivel2()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	matTab = LoadObject<UMaterialInterface>(nullptr, TEXT("Material'/Game/Mesh/Material_1.Material_1'"));
}

// Called when the game starts or when spawned
void ANivel2::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ANivel2::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ANivel2::CambiarNivel(ACambiarNiv* _Nivel)
{
	_Nivel->TableroMesh->SetMaterial(0, matTab);
	_Nivel->TableroMesh->SetWorldScale3D(FVector(1.0f, 1.0f, 1.0f));
	Nivel->CambiarNEstado(Nivel->getNivel1());
}

void ANivel2::SetNivel(ACambiarNiv* _Nivel)
{
	Nivel = _Nivel;
}

