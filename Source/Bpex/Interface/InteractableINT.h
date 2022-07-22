// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InteractableINT.generated.h"

class APlayerCharacter;

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UInteractableINT : public UInterface
{
	GENERATED_BODY()
};

/**
 *
 */
class BPEX_API IInteractableINT
{
	GENERATED_BODY()

	DECLARE_DELEGATE(FInteractDel)
	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintNativeEvent)
		void InteractINTFunc(APlayerCharacter* player);
	virtual void InteractINTFunc_Implementation(APlayerCharacter* player) = 0;
};
