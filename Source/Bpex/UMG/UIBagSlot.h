// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UIBagSlot.generated.h"


UCLASS()
class BPEX_API UUIBagSlot : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UImage* ItemImg;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock* ItemNum;

public:
	UFUNCTION(BlueprintCallable)
	void SetSlotImg(UTexture2D* img);

	UFUNCTION(BlueprintCallable)
	void SetSlotText(int32 num);
};
