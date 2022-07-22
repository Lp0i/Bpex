// Fill out your copyright notice in the Description page of Project Settings.


#include "ObjInfoWidget.h"
#include "Components/HorizontalBox.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/Overlay.h"
#include "Components/CanvasPanelSlot.h"
#include "Engine/Texture.h"

void UObjInfoWidget::bindType(FText text)
{
	ObjName->SetText(text);
}

void UObjInfoWidget::bindNum(int32 num)
{
	FString text = TEXT("x");
	text.Append(FString::FromInt(num));
	Num->SetText(FText::FromString(text));
}

void UObjInfoWidget::bindImg(UTexture2D* img)
{
	ObjImg->SetBrushFromTexture(img, true);
}

void UObjInfoWidget::setBoxLoc(FVector2D NewLocation)
{
	auto BoxSlot = Cast<UCanvasPanelSlot>(HorizonPanel->Slot);
	if (BoxSlot)
	{
		BoxSlot->SetPosition(NewLocation);
	}
}
