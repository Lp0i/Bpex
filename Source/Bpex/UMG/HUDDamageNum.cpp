// Fill out your copyright notice in the Description page of Project Settings.
#include "HUDDamageNum.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"

bool UHUDDamageNum::Initialize()
{
	Super::Initialize();

	DamageCount = 0;
	DamageNum->SetRenderOpacity(1.f);
	DamageNum->SetText(FText::FromString(""));
	ArmourImg->SetRenderOpacity(1.f);

	//GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Red, TEXT("init"));
	return true;
}

void UHUDDamageNum::AddDamage(float damage, int32 level, bool bIsArmourBreak)
{
	DamageCount += damage;
	
	auto TimerManager = GetGameInstance()->TimerManager;
	if (IsPlayingAnimation())
	{
		StopAnimation(FadeAnim);
		TimerManager->ClearTimer(ResetTimer);
	}
	DamageNum->SetRenderOpacity(1.f);
	DamageNum->SetText(FText::AsNumber(DamageCount));
	if (!bIsArmourBreak)
	{
		DamageNum->Font.OutlineSettings.OutlineColor = GetColorByLevel(level);
		ArmourImg->SetRenderOpacity(1.f);
		ArmourImg->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f));
	}
	else
	{
		//GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Red, TEXT("break"));
		DamageNum->Font.OutlineSettings.OutlineColor = FLinearColor(0.89f, 0.13f, 0.11f);
		ArmourImg->SetRenderOpacity(0.f);
	}
		
	if (TimerManager->IsTimerActive(CountTimer))
	{
		TimerManager->ClearTimer(CountTimer);
		TimerManager->SetTimer(CountTimer, this, &UHUDDamageNum::PlayFadeAnim, 0.3f, false);
	}
	else
	{
		TimerManager->SetTimer(CountTimer, this, &UHUDDamageNum::PlayFadeAnim, 0.3f, false);
	}
}

void UHUDDamageNum::PlayFadeAnim()
{
	PlayAnimationAtTime(FadeAnim, 0.f);

	auto TimerManager = GetGameInstance()->TimerManager;
	TimerManager->SetTimer(ResetTimer, this, &UHUDDamageNum::ResetCount, FadeAnim->GetEndTime(), false);
};

FLinearColor UHUDDamageNum::GetColorByLevel(int32 level)
{
	FLinearColor RetColor;
	switch (level)
	{
	case 1:
		RetColor = FLinearColor(0.67f, 0.67f, 0.67f);
		break;

	case 2:
		RetColor = FLinearColor(0.f, 0.57f, 0.85f);
		break;

	case 3:
		RetColor = FLinearColor(0.75f, 0.f, 0.85f);
		break;

	case 4:
		RetColor = FLinearColor(0.85f, 0.027f, 0.f);
		break;
	}

	return RetColor;
}