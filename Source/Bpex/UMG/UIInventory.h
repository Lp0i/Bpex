// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Bpex/Object/WeaponBase.h"
#include "UIInventory.generated.h"

class UUIBagPanel;
class UUIWeaponPanel;

UCLASS()
class BPEX_API UUIInventory : public UUserWidget
{
	GENERATED_BODY()

public:
	bool bIsShowed = false;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UUIBagPanel* BagPanel;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UUIWeaponPanel* WeaponPanel_1;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UUIWeaponPanel* WeaponPanel_2;

public:
	UFUNCTION(BlueprintCallable)
		void SetupWidget();

	UFUNCTION()
		void UpdateBagPanel(TArray<AItemBase*>& BagList, int32 BagSize);
	UFUNCTION()
		void UpdateWeaponPanel(FWeaponInfo& WeaponInfo, int32 UseIdx);

	UFUNCTION(BlueprintCallable)
		void InputShowOrHide();
};
