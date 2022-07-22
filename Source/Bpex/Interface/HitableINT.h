// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "HitableINT.generated.h"

UENUM(BlueprintType)
enum class EDamageType :uint8
{
	COMMON,
	ARMOUR_ONLY,
	HEALTH_ONLY,
};

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UHitableINT : public UInterface
{
	GENERATED_BODY()
};

/**
 *
 */
class BPEX_API IHitableINT
{
	GENERATED_BODY()

		// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintNativeEvent, Category = "Hit")
	bool HitINTFunc(float damage, const FVector& hitLocation, const FString& hitBoneName, EDamageType DamageType);
	virtual bool HitINTFunc_Implementation(float Damage, const FVector& hitLocation, const FString& hitBoneName = TEXT("None"), EDamageType DamageType = EDamageType::COMMON) = 0;
};
