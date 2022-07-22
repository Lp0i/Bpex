// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PickupINT.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UPickupINT : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class BPEX_API IPickupINT
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintNativeEvent, Category = "Trace")
	void OnVisibleLineTraceINTFunc();
	virtual void OnVisibleLineTraceINTFunc_Implementation() = 0;

	UFUNCTION(BlueprintNativeEvent, Category = "Trace")
	void HideInfoINTFunc();
	virtual void HideInfoINTFunc_Implementation() = 0;
};
