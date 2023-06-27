// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InterfaceNivel.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UInterfaceNivel : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class TETRIS_UE4_API IInterfaceNivel
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void CambiarNivel(class ACambiarNiv* _Nivel) = 0;
	virtual void SetNivel(class ACambiarNiv* Nivel) = 0;
};
