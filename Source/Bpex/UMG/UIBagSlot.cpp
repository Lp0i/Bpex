// Fill out your copyright notice in the Description page of Project Settings.


#include "UIBagSlot.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Blueprint/WidgetLayoutLibrary.h"

void UUIBagSlot::SetSlotImg(UTexture2D* Img)
{
	if (Img != nullptr)
	{
		ItemImg->SetBrushFromTexture(Img, false);
		ItemImg->SetRenderOpacity(1.f);
	}
	else
	{
		ItemImg->SetBrushFromTexture(nullptr, false);
		ItemImg->SetRenderOpacity(0.f);
	}
}

void UUIBagSlot::SetSlotText(int32 num)
{
	if (num != 0)
		ItemNum->SetText(FText::AsNumber(num));
	else
		ItemNum->SetText(FText::FromString(""));
}

void UUIBagSlot::ClearSlotData()
{
	ItemRef = nullptr;
	SetSlotText(0);
	SetSlotImg(nullptr);
}