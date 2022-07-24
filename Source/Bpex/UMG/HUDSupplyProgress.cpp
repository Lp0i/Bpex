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
		//GEngine->AddOnScreenDebugMessage(-1, 0.5, FColor::Blue, FString::SanitizeFloat(suppliedTime));
		SetProgress(suppliedTime / SupplyTimeLength);
	}
}

void UHUDSupplyProgress::SetupSupplyData(float supplyTime, UTexture2D* img, FString name)
{
	GetGameInstance()->GetTimerManager().SetTimer(SupplyingTimer, this, &UHUDSupplyProgress::SupplyFinished, supplyTime, false);
	SupplyTimeLength = supplyTime;
	bIsTicking = true;
	SetSupplementImg(img);
	SetSupplementName(name);
	SetProgress(0.f);
	SetRenderOpacity(1.f);
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