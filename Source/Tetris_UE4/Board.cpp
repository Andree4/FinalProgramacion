// Fill out your copyright notice in the Description page of Project Settings.


#include "Board.h"
#include "EngineUtils.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"


// Sets default values
ABoard::ABoard()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    static ConstructorHelpers::FObjectFinder<USoundCue> LineRemove_Sound(TEXT("SoundCue'/Game/Sounds/line-remove_Cue.line-remove_Cue'"));
    if(LineRemove_Sound.Succeeded())
    {
        LineRemoveSoundCue = LineRemove_Sound.Object;
    }
    
    static ConstructorHelpers::FObjectFinder<USoundCue> MoveToEnd_Sound(TEXT("SoundCue'/Game/Sounds/force-hit_Cue.force-hit_Cue'"));
    if(MoveToEnd_Sound.Succeeded())
    {
        MoveToEndSoundCue = MoveToEnd_Sound.Object;
    }
    
    static ConstructorHelpers::FObjectFinder<USoundCue> GameStart_Sound(TEXT("SoundCue'/Game/Sounds/start_Cue.start_Cue'"));
    if(GameStart_Sound.Succeeded())
    {
        GameStartSoundCue = GameStart_Sound.Object;
    }
    
    static ConstructorHelpers::FObjectFinder<USoundCue> GameOver_Sound(TEXT("SoundCue'/Game/Sounds/gameover_Cue.gameover_Cue'"));
    if(GameOver_Sound.Succeeded())
    {
        GameOverSoundCue = GameOver_Sound.Object;
    }
}

// Called when the game starts or when spawned
void ABoard::BeginPlay()
{
	Super::BeginPlay();
    Nivel = GetWorld()->SpawnActor<ACambiarNiv>(ACambiarNiv::StaticClass());
    Niv = Nivel->inicio();
    for(TActorIterator<APieces> it(GetWorld()); it; ++it)
    {
        if(it->GetFName() == TEXT("DissmissPieces"))
        {
            it->Dismiss();
            it->Destroy();
        }
    }
}

// Called every frame
void ABoard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
    
    if(bGameOver)
    {
        return;
    }

    switch(Status)
    {
        case PS_NOT_INITED:
            NewPieces();
            CoolLeft = CoolDown;
            Status = PS_MOVING;
            break;
        case PS_MOVING:
            CoolLeft -= DeltaTime;
            if(CoolLeft <= 0.0f)
            {
                MoveDown();
            }
            break;
        case PS_GOT_BOTTOM:
            CoolLeft -= DeltaTime;
            if(CoolLeft <= 0.0f)
            {
                MoverPiezaa();
                CoolLeft = CoolDown;
                Status = PS_MOVING;
            }
            break;
        default:
            break;
    }
}

// Called to bind functionality to input
void ABoard::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

    PlayerInputComponent->BindAction("Rotate", IE_Pressed, this, &ABoard::Rotate);
    PlayerInputComponent->BindAction("MoveLeft", IE_Pressed, this, &ABoard::MoveLeft);
    PlayerInputComponent->BindAction("MoveRight", IE_Pressed, this, &ABoard::MoveRight);
    PlayerInputComponent->BindAction("MoveDownToEnd", IE_Pressed, this, &ABoard::MoveDownToEnd);
    PlayerInputComponent->BindAction("FinJuego", IE_Pressed, this, &ABoard::FinJuego);
    PlayerInputComponent->BindAction("CambiarNivel", IE_Pressed, this, &ABoard::CambiarNivel);
    PlayerInputComponent->BindAction("Reinicio", IE_Pressed, this, &ABoard::Reinicio);

    //PlayerInputComponent->BindAction("NewPieces", IE_Pressed, this, &ABoard::NewPieces);
    //PlayerInputComponent->BindAction("CheckLine", IE_Pressed, this, &ABoard::CheckLine);
}
void ABoard::CambiarNivel()
{ 
    Nivel->CambiarNivel(Niv);
    if (band == 0) {
        band = 1;
    }
    else {
        band = 0;
    }
}

void ABoard::Rotate()
{
    if(CurrentPieces && Status != PS_GOT_BOTTOM)
    {
        CurrentPieces->TestRotate();
    }
}

void ABoard::MoveLeft()
{
    if(CurrentPieces)
    {
        CurrentPieces->MoveLeft();
        if(Status == PS_GOT_BOTTOM)
        {
            MoveDownToEnd();
        }
    }
}

void ABoard::MoveRight()
{
    if(CurrentPieces)
    {
        CurrentPieces->MoveRight();
        if(Status == PS_GOT_BOTTOM)
        {
            MoveDownToEnd();
        }
    }
}

void ABoard::MoveDown()
{
    if(CurrentPieces)
    {
        if(!CurrentPieces->MoveDown())
        {
            
            Status = PS_GOT_BOTTOM;
        }
        CoolLeft = CoolDown;
    }
}

void ABoard::NewPieces()
{
    if (band == 0) {
        CheckLine();
    }
    else {
        CheckLine2();
    }

    if(CurrentPieces)
    {
        CurrentPieces->Dismiss();
        CurrentPieces->Destroy();
    }
    if (SigPiece)
    {
        SigPiece->Dismiss();
        SigPiece->Destroy();
    }
    if (SubSPiece)
    {
        SubSPiece->Dismiss();
        SubSPiece->Destroy();
    }
    
    FRotator Rotation(0.0, 0.0, 0.0);
   
        FVector Location(0.0, 5.0, 195.0);
        CurrentPieces = GetWorld()->SpawnActor<APieces>(Location, Rotation);

        FVector Location2(0.0, -130.0, 170.0);
        SigPiece = GetWorld()->SpawnActor<APieces>(Location2, Rotation);

        FVector Location3(0.0, -130.0, 110.0);
        SubSPiece = GetWorld()->SpawnActor<APieces>(Location3, Rotation);
    
    
    bGameOver = CheckGameOver();
    if(bGameOver)
    {
        UE_LOG(LogTemp, Warning, TEXT("Game Over!!!!!!!!"));
        if(GameOverSoundCue)
        {
            UGameplayStatics::PlaySoundAtLocation(GetWorld(), GameOverSoundCue, GetActorLocation(), GetActorRotation());
        }
    }
    
}
void ABoard::MoverPiezaa()
{
    FString Contador1 = FString("Numero de Piezas: ") + FString::FromInt(NPiezas);
    GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Yellow, Contador1);

    if (band == 0) {
        CheckLine();
    }
    else {
        CheckLine2();
    }
    FRotator Rotation(0.0, 0.0, 0.0);
    if (CurrentPieces) {

        CurrentPieces = SigPiece;
        FVector Location(0.0, 5.0, 195.0);
        CurrentPieces->SetActorLocation(Location);

        // SubsPiece se convierte en SigPiece
        SigPiece = SubSPiece;
        FVector Location2(0.0, -140.0, 170.0);
        SigPiece->SetActorLocation(Location2);

        // Crear una nueva SubsPiece
        FVector Location3(0.0, -130.0, 110.0);
        SubSPiece = GetWorld()->SpawnActor<APieces>(Location3, Rotation);
        NPiezas = NPiezas + 1;
    }
    bGameOver = CheckGameOver();
    if (bGameOver)
    {
        UE_LOG(LogTemp, Warning, TEXT("Game Over!!!!!!!!"));
        if (GameOverSoundCue)
        {
            UGameplayStatics::PlaySoundAtLocation(GetWorld(), GameOverSoundCue, GetActorLocation(), GetActorRotation());
        }
    }
}



void ABoard::CheckLine()
{
    auto MoveDownFromLine = [this](int z){
        FVector Location(0.0f, 0.0f, 5.0*z + 100.0);
        FRotator Rotation(0.0f, 0.0f, 0.0f);
        TArray<struct FOverlapResult> OutOverlaps;
        FCollisionShape CollisionShape;
            FVector Extent(4.5f, 49.5f, 95.0 + 4.5 - 5.0 * z);
        
        CollisionShape.SetBox(Extent);
        DrawDebugBox(GetWorld(), Location, Extent, FColor::Blue, false, 1, 0, 1);
        FCollisionQueryParams Params;
        FCollisionResponseParams ResponseParam;
        if(GetWorld()->OverlapMultiByChannel(OutOverlaps,
            Location, Rotation.Quaternion(), ECollisionChannel::ECC_WorldDynamic,
            CollisionShape, Params, ResponseParam))
        {
            for(auto&& Result: OutOverlaps)
            {
                FVector NewLocation = Result.GetActor()->GetActorLocation();
                NewLocation.Z -= 10.0f;
                Result.GetActor()->SetActorLocation(NewLocation);
            }
        }
    };
    
    int z = 0;
    while(z < 20)
    {
        FVector Location(0.0f, 0.0f, 10.0f * z + 5.0f);
        FRotator Rotation(0.0f, 0.0f, 0.0f);
        TArray<struct FOverlapResult> OutOverlaps;
        FCollisionShape CollisionShape;
        if (band == 0) {
            CollisionShape.SetBox(FVector(4.0f, 49.0f, 4.0f));
        }
        else {
            CollisionShape.SetBox(FVector(4.0f, 98.0f, 4.0f));
        }
        //DrawDebugBox(GetWorld(), Location, FVector(4.5f, 49.5f, 4.5f), FColor::Purple, false, 1, 0, 1);
        FCollisionQueryParams Params;
        FCollisionResponseParams ResponseParam;
        bool b = GetWorld()->OverlapMultiByChannel(OutOverlaps,
            Location, Rotation.Quaternion(), ECollisionChannel::ECC_WorldDynamic,
            CollisionShape, Params, ResponseParam);
        if(!b || OutOverlaps.Num() < 10)
        {
            ++z;
            continue;
        }
        else // this line is full, remove the line
        {
            NLineas = NLineas + 1;
            
            FString Cont2 = FString("Numero de Lineas Completadas: ") + FString::FromInt(NLineas);
            GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Blue, Cont2);

                NBlockD = NBlockD + 10;
                Puntaje = Puntaje + 50.0;
                FString Cont3 = FString("Numero de Bloques Destruidos: ") + FString::FromInt(NBlockD);
                GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Green, Cont3);

                FString Punt1 = FString("Puntaje Total: ") + FString::FromInt(Puntaje);
                GEngine->AddOnScreenDebugMessage(-1, 10, FColor::White, Punt1);
            
            

            UE_LOG(LogTemp, Warning, TEXT("Find FULL LINE at z=%d"), z);
            for(auto&& Result: OutOverlaps)
            {
                Result.GetActor()->Destroy();
            }
            MoveDownFromLine(z);
            
            if(LineRemoveSoundCue)
            {
                UGameplayStatics::PlaySoundAtLocation(GetWorld(), LineRemoveSoundCue, GetActorLocation(), GetActorRotation());
            }
        }
    }
}

void ABoard::CheckLine2()
{
    auto MoveDownFromLine = [this](int z) {
        FVector Location(0.0f, 0.0f, 5.0 * z + 100.0);
        FRotator Rotation(0.0f, 0.0f, 0.0f);
        TArray<struct FOverlapResult> OutOverlaps;
        FCollisionShape CollisionShape;
            FVector Extent(4.5f, 99.0f, 95.0 + 4.5 - 5.0 * z);
        
        CollisionShape.SetBox(Extent);
        DrawDebugBox(GetWorld(), Location, Extent, FColor::Red, false, 1, 0, 1);
        FCollisionQueryParams Params;
        FCollisionResponseParams ResponseParam;
        if (GetWorld()->OverlapMultiByChannel(OutOverlaps,
            Location, Rotation.Quaternion(), ECollisionChannel::ECC_WorldDynamic,
            CollisionShape, Params, ResponseParam))
        {
            for (auto&& Result : OutOverlaps)
            {
                FVector NewLocation = Result.GetActor()->GetActorLocation();
                NewLocation.Z -= 10.0f;
                Result.GetActor()->SetActorLocation(NewLocation);
            }
        }
    };

    int z = 0;
    while (z < 40)
    {
        FVector Location(0.0f, 0.0f, 10.0f * z + 5.0f);
        FRotator Rotation(0.0f, 0.0f, 0.0f);
        TArray<struct FOverlapResult> OutOverlaps;
        FCollisionShape CollisionShape;
            CollisionShape.SetBox(FVector(4.0f, 98.0f, 4.0f));
        
        //DrawDebugBox(GetWorld(), Location, FVector(4.5f, 49.5f, 4.5f), FColor::Purple, false, 1, 0, 1);
        FCollisionQueryParams Params;
        FCollisionResponseParams ResponseParam;
        bool b = GetWorld()->OverlapMultiByChannel(OutOverlaps,
            Location, Rotation.Quaternion(), ECollisionChannel::ECC_WorldDynamic,
            CollisionShape, Params, ResponseParam);
        if (!b || OutOverlaps.Num() < 20)
        {
            ++z;
            continue;
        }
        else // this line is full, remove the line
        {
            NLineas = NLineas + 1;

            FString Cont2 = FString("Numero de Lineas Completadas: ") + FString::FromInt(NLineas);
            GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Blue, Cont2);

                NBlockD = NBlockD + 20;
                Puntaje = Puntaje + 100.0;

                FString Cont4 = FString("Numero de Bloques Destruidos: ") + FString::FromInt(NBlockD);
                GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Green, Cont4);

                FString Punt2 = FString("Puntaje Total: ") + FString::FromInt(Puntaje);
                GEngine->AddOnScreenDebugMessage(-1, 10, FColor::White, Punt2);
            


            UE_LOG(LogTemp, Warning, TEXT("Find FULL LINE at z=%d"), z);
            for (auto&& Result : OutOverlaps)
            {
                Result.GetActor()->Destroy();
            }
            MoveDownFromLine(z);

            if (LineRemoveSoundCue)
            {
                UGameplayStatics::PlaySoundAtLocation(GetWorld(), LineRemoveSoundCue, GetActorLocation(), GetActorRotation());
            }
        }
    }
}



void ABoard::MoveDownToEnd()
{
    if(!CurrentPieces)
    {
        return;
    }
    
    while(CurrentPieces->MoveDown())
    {
    }
    
    if(MoveToEndSoundCue)
    {
        UGameplayStatics::PlaySoundAtLocation(GetWorld(), MoveToEndSoundCue, GetActorLocation(), GetActorRotation());
    }
    
    switch(Status)
    {
        case PS_MOVING:
            Status = PS_GOT_BOTTOM;
            CoolLeft = CoolDown;
            break;
        case PS_GOT_BOTTOM:
            break;
        default:
            UE_LOG(LogTemp, Warning, TEXT("Wrong status for MoveDownToEnd"));
            break;
    }
}

void ABoard::FinJuego()
{
    CurrentPieces = nullptr;
    CheckGameOver();
    if (GameOverSoundCue)
    {
        UGameplayStatics::PlaySoundAtLocation(GetWorld(), GameOverSoundCue, GetActorLocation(), GetActorRotation());
    }
}

void ABoard::Reinicio()
{
    auto MoveDownFromLine = [this](int z) {
        FVector Location(0.0f, 0.0f, 5.0 * z + 100.0);
        FRotator Rotation(0.0f, 0.0f, 0.0f);
        TArray<struct FOverlapResult> OutOverlaps;
        FCollisionShape CollisionShape;
        FVector Extent;
        if (band == 0) {
            FVector Extent(4.5f, 49.5f, 95.0 + 4.5 - 5.0 * z);
        }
        else {
            FVector Extent(4.5f, 99.0f, 95.0 + 4.5 - 5.0 * z);
        }
        CollisionShape.SetBox(Extent);
        DrawDebugBox(GetWorld(), Location, Extent, FColor::Purple, false, 1, 0, 1);
        FCollisionQueryParams Params;
        FCollisionResponseParams ResponseParam;
        if (GetWorld()->OverlapMultiByChannel(OutOverlaps,
            Location, Rotation.Quaternion(), ECollisionChannel::ECC_WorldDynamic,
            CollisionShape, Params, ResponseParam))
        {
            for (auto&& Result : OutOverlaps)
            {
                FVector NewLocation = Result.GetActor()->GetActorLocation();
                NewLocation.Z -= 10.0f;
                Result.GetActor()->SetActorLocation(NewLocation);
            }
        }
    };

    int z = 0;
    while (z < 40)
    {
        FVector Location(0.0f, 0.0f, 10.0f * z + 5.0f);
        FRotator Rotation(0.0f, 0.0f, 0.0f);
        TArray<struct FOverlapResult> OutOverlaps;
        FCollisionShape CollisionShape;
        if (band == 0) {
            CollisionShape.SetBox(FVector(4.0f, 49.0f, 4.0f));
        }
        else {
            CollisionShape.SetBox(FVector(4.0f, 98.0f, 4.0f));
        }
        
        //DrawDebugBox(GetWorld(), Location, FVector(4.5f, 49.5f, 4.5f), FColor::Purple, false, 1, 0, 1);
        FCollisionQueryParams Params;
        FCollisionResponseParams ResponseParam;
        bool b = GetWorld()->OverlapMultiByChannel(OutOverlaps,
            Location, Rotation.Quaternion(), ECollisionChannel::ECC_WorldDynamic,
            CollisionShape, Params, ResponseParam);
        if (!b || OutOverlaps.Num() < 1)
        {
            ++z;
            continue;
        }
        else // this line is full, remove the line
        {
            UE_LOG(LogTemp, Warning, TEXT("Find FULL LINE at z=%d"), z);
            for (auto&& Result : OutOverlaps)
            {
                Result.GetActor()->Destroy();
            }
            MoveDownFromLine(z);

            if (LineRemoveSoundCue)
            {
                UGameplayStatics::PlaySoundAtLocation(GetWorld(), LineRemoveSoundCue, GetActorLocation(), GetActorRotation());
            }
        }
    }
    Status = PS_NOT_INITED;
}

bool ABoard::CheckGameOver()
{
    if(!CurrentPieces)
    {
        UE_LOG(LogTemp, Warning, TEXT("NoPieces"));
        return true;
    }
    
    return CurrentPieces->CheckWillCollision([](FVector OldVector){ return OldVector; });
}
