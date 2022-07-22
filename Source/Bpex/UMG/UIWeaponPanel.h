// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Bpex/Object/WeaponBase.h"
#include "UIWeaponPanel.generated.h"

/**
 *
 */
UCLASS()
class BPEX_API UUIWeaponPanel : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UImage* WeaponImg;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock* WeaponName;

public:
	void UpdateWeaponUI(FWeaponInfo& weaponInfo);
};
