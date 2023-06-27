// Fill out your copyright notice in the Description page of Project Settings.


#include "CambiarNiv.h"
#include "InterfaceNivel.h"
#include "Nivel1.h"
#include "Nivel2.h"
// Sets default values
ACambiarNiv::ACambiarNiv()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    TableroMesh = CreateDefaultSubobject<UStaticMeshComponent>("TableroMesh");
    RootComponent = TableroMesh;

    static ConstructorHelpers::FObjectFinder<UStaticMesh> Niveles(TEXT("StaticMesh'/Game/Mesh/frame.frame'"));
   
    if (Niveles.Succeeded())
    {
        TableroMesh->SetStaticMesh(Niveles.Object);
        TableroMesh->SetRelativeLocation(FVector(1000.0, 1000.0, 1000.0));
        TableroMesh->SetWorldScale3D(FVector(1.0f, 1.0f, 1.0f));
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("mesh not found"));
    }

    matTab = LoadObject<UMaterialInterface>(nullptr, TEXT("Material'/Game/Mesh/Material_1.Material_1'"));
}

// Called when the game starts or when spawned
void ACambiarNiv::BeginPlay()
{
	Super::BeginPlay();
	
}

ACambiarNiv* ACambiarNiv::inicio()
{
    Nivel1 = GetWorld()->SpawnActor<ANivel1>(ANivel1::StaticClass());
    Nivel1->SetNivel(this);

    Nivel2 = GetWorld()->SpawnActor<ANivel2>(ANivel2::StaticClass());
    Nivel2->SetNivel(this);

    FTransform SpawnLocation;
    
    ACambiarNiv* Nivel = GetWorld()->SpawnActor<ACambiarNiv>(ACambiarNiv::StaticClass(), SpawnLocation);
    Nivel->TableroMesh->SetMaterial(0, matTab);

    //B->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
    Nivel->SetActorRelativeLocation(FVector(0.0, 0.0, 0.0));
    Nivel->SetActorRelativeRotation(FRotator(0.000000, 0.000000, 0.000000));
    Estado = Nivel1;
    return Nivel;
}

void ACambiarNiv::CambiarNivel(ACambiarNiv* _Nivel)
{
    Estado->CambiarNivel(_Nivel);
}


void ACambiarNiv::CambiarNEstado(IInterfaceNivel* _Estado)
{
    Estado = _Estado;
}

IInterfaceNivel* ACambiarNiv::getNivel1()
{
    return Nivel1;
}

IInterfaceNivel* ACambiarNiv::getNivel2()
{
    return Nivel2;
}

IInterfaceNivel* ACambiarNiv::getEstado()
{
    return Estado;
}

// Called every frame
void ACambiarNiv::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

