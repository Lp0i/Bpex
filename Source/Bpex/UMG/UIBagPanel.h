// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UIBagSlot.h"
#include "Bpex/Object/ItemBase.h"
#include "UIBagPanel.generated.h"

class AItemBase;

UCLASS()
class BPEX_API UUIBagPanel : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UUIBagSlot* Slot_1;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UUIBagSlot* Slot_2;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UUIBagSlot* Slot_3;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UUIBagSlot* Slot_4;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UUIBagSlot* Slot_5;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UUIBagSlot* Slot_6;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UUIBagSlot* Slot_7;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UUIBagSlot* Slot_8;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UUIBagSlot* Slot_9;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UUIBagSlot* Slot_10;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UUIBagSlot* Slot_11;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UUIBagSlot* Slot_12;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UUIBagSlot* Slot_13;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UUIBagSlot* Slot_14;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UUIBagSlot* Slot_15;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UUIBagSlot* Slot_16;

public:
	UFUNCTION(BlueprintCallable)
		void UpdateBagUI(TArray<AItemBase*>& BagList, int32 BagSize);

};
