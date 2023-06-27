// Fill out your copyright notice in the Description page of Project Settings.


#include "Pieces.h"
#include "Components/SceneComponent.h"
#include "DrawDebugHelpers.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"
#include <vector>

// Sets default values
APieces::APieces()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    SceneComponent = CreateDefaultSubobject<USceneComponent>("Pieces Scene");
    RootComponent = SceneComponent;
    
    static FConstructorStatics ConstructorStatics;
    Colors.Add(ConstructorStatics.Color_0.Get());
    Colors.Add(ConstructorStatics.Color_1.Get());
    Colors.Add(ConstructorStatics.Color_2.Get());
    Colors.Add(ConstructorStatics.Color_3.Get());
    Colors.Add(ConstructorStatics.Color_4.Get());
    Colors.Add(ConstructorStatics.Color_5.Get());
    Colors.Add(ConstructorStatics.Color_6.Get());
    
    static ConstructorHelpers::FObjectFinder<USoundCue> Rotate_Sound(TEXT("SoundCue'/Game/Sounds/block-rotate_Cue.block-rotate_Cue'"));
    if(Rotate_Sound.Succeeded())
    {
        RotateSoundCue = Rotate_Sound.Object;
    }
    
    static ConstructorHelpers::FObjectFinder<USoundCue> MoveDown_Sound(TEXT("SoundCue'/Game/Sounds/slow-hit_Cue.slow-hit_Cue'"));
    if(MoveDown_Sound.Succeeded())
    {
        MoveLeftRightSoundCue = MoveDown_Sound.Object;
    }
}

// Called when the game starts or when spawned
void APieces::BeginPlay()
{
	Super::BeginPlay();
	
    SpawnBlocks();
}

// Called every frame
void APieces::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
void APieces::NumA()
{
    numa = FMath::RandRange(0, 7- 1);
}

void APieces::SpawnBlocks()
{
    
    std::vector<std::vector<std::pair<float, float>>> Shapes =
    {
        {{-20.0, 0.0}, {-10.0, 0.0}, {0.0, 0.0}, {10.0, 0.0}},
        {{0.0, 10.0}, {0.0, 0.0}, {10.0, 0.0}, {20.0, 0.0}},
        {{-20.0, 0.0}, {-10.0, 0.0}, {0.0, 0.0}, {0.0, 10.0}},
        {{0.0, 0.0}, {10.0, 0.0}, {0.0, -10.0}, {10.0, -10.0}},
        {{-10.0, -10.0}, {0.0, -10.0}, {0.0, 0.0}, {10.0, 0.0}},
        {{-10.0, 0.0}, {0.0, 0.0}, {0.0, 10.0}, {10.0, 0.0}},
        {{-10.0, 0.0}, {0.0, 0.0}, {0.0, -10.0}, {10.0, -10.0}},
    };
    NumA();
    UE_LOG(LogTemp, Warning, TEXT("numa=%d"), numa);
    const std::vector<std::pair<float, float>>& YZs = Shapes[numa];
    FRotator Rotation(0.0, 0.0, 0.0);

    for(auto&& YZ: YZs)
    {
        ABlock* B = nullptr;
        switch (numa) {
        case 1:
            B = GetWorld()->SpawnActor<ABlockBMadera>(this->GetActorLocation(), Rotation);
            B->Mesh->SetMaterial(1, Colors[0]);
            break;
        case 2:
            B = GetWorld()->SpawnActor<ABlockBAgua>(this->GetActorLocation(), Rotation);
            B->Mesh->SetMaterial(1, Colors[1]);
            break;
        case 3:
            B = GetWorld()->SpawnActor<ABlockBBrillante>(this->GetActorLocation(), Rotation);
            B->Mesh->SetMaterial(1, Colors[2]);
            break;
        case 4:
            B = GetWorld()->SpawnActor<ABlockBMadera>(this->GetActorLocation(), Rotation);
            B->Mesh->SetMaterial(1, Colors[0]);
            break;
        case 5:
            B = GetWorld()->SpawnActor<ABlockBAgua>(this->GetActorLocation(), Rotation);
            B->Mesh->SetMaterial(1, Colors[1]);
            break;
        case 6:
            B = GetWorld()->SpawnActor<ABlockBPiedra>(this->GetActorLocation(), Rotation);
            B->Mesh->SetMaterial(1, Colors[3]);
            break;
        default:
            B = GetWorld()->SpawnActor<ABlockBBrillante>(this->GetActorLocation(), Rotation);
            B->Mesh->SetMaterial(1, Colors[2]);
            break;
        }
        
        Blocks.Add(B);
        B->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
        B->SetActorRelativeLocation(FVector(0.0, YZ.first, YZ.second));
    }
}

void APieces::DrawDebugLines()
{
    for(ABlock* B: Blocks)
    {
        FVector V1 = B->GetActorLocation();
        FVector V2(V1.X, V1.Y, 5.0f);
        DrawDebugLine(GetWorld(), V1, V2, FColor::Green, false, 1, 0, 1);
    }
}

void APieces::TestRotate()
{
    auto RotateVector = [this](FVector OldVector){
        FVector ActorVector = this->GetActorLocation();
        FVector BlockVector = OldVector;
        FVector TempVector = BlockVector - ActorVector;
        TempVector = ActorVector + TempVector.RotateAngleAxis(90.0, FVector(1.0, 0.0, 0.0));
        return TempVector;
    };
    
    if(!CheckWillCollision(RotateVector))
    {
        UE_LOG(LogTemp, Warning, TEXT("now can rotate"));
        FRotator NewRotation =  this->GetActorRotation() + FRotator(0.0, 0.0, -90.0);
        this->SetActorRelativeRotation(NewRotation);
        if(RotateSoundCue)
        {
            UGameplayStatics::PlaySoundAtLocation(GetWorld(), RotateSoundCue, GetActorLocation(), GetActorRotation());
        }
    }
}

void APieces::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    UE_LOG(LogTemp, Warning, TEXT("Pieces Deleted"));
}

void APieces::Dismiss()
{
    Blocks.Empty();
}

void APieces::MoveLeft()
{
    auto MoveVectorLeft = [](FVector OldVector){
        OldVector.Y -= 10.0f;
        return OldVector;
    };
    
    if(!CheckWillCollision(MoveVectorLeft))
    {
        FVector NewLocation = GetActorLocation();
        NewLocation.Y -= 10;
        SetActorLocation(NewLocation);
        
        if(MoveLeftRightSoundCue)
        {
            UGameplayStatics::PlaySoundAtLocation(GetWorld(), MoveLeftRightSoundCue, GetActorLocation(), GetActorRotation());
        }
    }
}

void APieces::MoveRight()
{
    auto MoveVectorRight = [](FVector OldVector){
        OldVector.Y += 10.0f;
        return OldVector;
    };

    if(!CheckWillCollision(MoveVectorRight))
    {
        FVector NewLocation = GetActorLocation();
        NewLocation.Y += 10;
        SetActorLocation(NewLocation);
        
        if(MoveLeftRightSoundCue)
        {
            UGameplayStatics::PlaySoundAtLocation(GetWorld(), MoveLeftRightSoundCue, GetActorLocation(), GetActorRotation());
        }
    }
}

bool APieces::MoveDown(bool PlaySound)
{
    auto MoveVectorDown = [](FVector OldVector){
        OldVector.Z -= 10.0f;
        return OldVector;
    };
    
    if(!CheckWillCollision(MoveVectorDown))
    {
        FVector NewLocation = GetActorLocation();
        NewLocation.Z -= 10;
        SetActorLocation(NewLocation);
        
        return true;
    }
    else
    {
        return false;
    }
}

bool APieces::CheckWillCollision(std::function<FVector(FVector OldLocation)> ChangeBeforeCheck)
{
    FCollisionQueryParams Params;
    for(ABlock* B: Blocks)
    {
        Params.AddIgnoredActor(B);
    }
    
    for(ABlock* B: Blocks)
    {
        FVector TempVector = B->GetActorLocation();
        TempVector = ChangeBeforeCheck(TempVector);
        
        TArray<struct FOverlapResult> OutOverlaps;
        FCollisionShape CollisionShape;
        CollisionShape.SetBox(FVector(4.0f, 4.0f, 4.0f));
        FCollisionResponseParams ResponseParam;
        bool b = GetWorld()->OverlapMultiByChannel(OutOverlaps,
            TempVector, B->GetActorQuat(), ECollisionChannel::ECC_WorldDynamic,
            CollisionShape, Params, ResponseParam);
        
        if(b)
        {
            for(auto&& Result: OutOverlaps)
            {
                UE_LOG(LogTemp, Warning, TEXT("OverLapped with actor %s, component=%s"), *Result.GetActor()->GetName(),
                       *Result.GetComponent()->GetName());
            }
            return true;
        }
    }
    
    return false;
}
void APieces::MoverPieza(FVector NewPosition)
{
    SetActorLocation(NewPosition);
}


