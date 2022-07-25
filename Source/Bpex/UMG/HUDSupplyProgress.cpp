// Fill out your copyright notice in the Description page of Project Settings.


#include "HUDSupplyProgress.h"
#include "Components/ProgressBar.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

void UHUDSupplyProgress::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (bIsTicking)
	{
		float suppliedTime = GetGameInstance()->GetTimerManager().GetTimerElapsed(SupplyingTimer);
		SetProgress(suppliedTime / SupplyTimeLength);
	}
}

void UHUDSupplyProgress::SetupSupplyData(float supplyTime, UTexture2D* img, FString name)
{
	if (supplyTime <= 0)
		SupplyFinished();
	else
	{
		GetGameInstance()->GetTimerManager().SetTimer(SupplyingTimer, this, &UHUDSupplyProgress::SupplyFinished, supplyTime, false);
		SupplyTimeLength = supplyTime;
		SetSupplementImg(img);
		SetSupplementName(name);
		SetProgress(0.f);
		SetRenderOpacity(1.f);
		bIsTicking = true;
	}
}

void UHUDSupplyProgress::SetSupplementImg(UTexture2D* img)
{
	SupplementImg->SetBrushFromTexture(img, false);
}

void UHUDSupplyProgress::SetSupplementName(const FString& name)
{
	SupplementName->SetText(FText::FromString(name));
}

void UHUDSupplyProgress::SetProgress(float percent)
{
	SupplyProgressBar->SetPercent(percent);
}

void UHUDSupplyProgress::SupplyFinished()
{
	bIsTicking = false;
	SetRenderOpacity(0.f);
}