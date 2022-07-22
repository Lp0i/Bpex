// Fill out your copyright notice in the Description page of Project Settings.


#include "HUDWidget.h"
#include "Bpex/Character/PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Components/Image.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"


void UHUDWidget::SetupWidget()
{
	player = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));

	if (player == nullptr)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, TEXT("Can't get Player!"));
		return;
	}

	SlotXL = Cast<UCanvasPanelSlot>(CrosshairXL->Slot);
	OriginPositionXL = SlotXL->GetPosition();
	SlotXR = Cast<UCanvasPanelSlot>(CrosshairXR->Slot);
	OriginPositionXR = SlotXR->GetPosition();
	SlotYU = Cast<UCanvasPanelSlot>(CrosshairYU->Slot);
	OriginPositionYU = SlotYU->GetPosition();
	SlotYD = Cast<UCanvasPanelSlot>(CrosshairYD->Slot);
	OriginPositionYD = SlotYD->GetPosition();

	SlotLU = Cast<UCanvasPanelSlot>(HitMarkLU->Slot);
	SlotRU = Cast<UCanvasPanelSlot>(HitMarkRU->Slot);
	SlotLD = Cast<UCanvasPanelSlot>(HitMarkLD->Slot);
	SlotRD = Cast<UCanvasPanelSlot>(HitMarkRD->Slot);
	OriginSizeHitMark = SlotLU->GetSize();
	HitMarkLength = 16.f;

	WeaponPanel->RenderOpacity = 0.f;

	auto playerBag = player->GetBagComp();
	auto playerInfo = player->GetInfoComp();
	if (playerBag != nullptr)
	{
		int32 ammoNum = playerBag->CountItemInBag(EItemType::AMMO);
		BagAmmo->SetText(FText::AsNumber(ammoNum));
		playerBag->HUDItemDel.BindUObject(this, &UHUDWidget::UpdateBagNum);
	}
	if (playerInfo != nullptr)
	{
		playerInfo->HUDInfoDel.BindUObject(this, &UHUDWidget::UpdateInfoPanel);
	}
	player->HUDWeaponInfoDel.AddUObject(this, &UHUDWidget::UpdateWeaponPanel);
	player->HUDAmmoDel.BindUObject(this, &UHUDWidget::UpdateAmmoNums);
	player->HUDHitMarkDel.BindUObject(this, &UHUDWidget::PopupHitMark);

	//ÉèÖÃHitMarkTimeLine
	if (HitMarkCurve != nullptr)
	{
		FOnTimelineFloatStatic updateFunc;
		FOnTimelineEventStatic finishFunc;
		updateFunc.BindUObject(this, &UHUDWidget::SetPopupValue);
		finishFunc.BindUObject(this, &UHUDWidget::OnHitPopupFinish);
		HitMarkTimeline.SetTimelineLength(0.07f);
		HitMarkTimeline.SetLooping(false);
		HitMarkTimeline.AddInterpFloat(HitMarkCurve, updateFunc);
		HitMarkTimeline.SetTimelineFinishedFunc(finishFunc);
		HitMarkTimeline.SetPropertySetObject(this);
		HitMarkTimeline.SetDirectionPropertyName(TEXT("HitMarkTimelineDirection"));
	}
}

void UHUDWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	HitMarkTimeline.TickTimeline(InDeltaTime);

	CrosshairTick(InDeltaTime);
}

void UHUDWidget::UpdateAmmoNums(int32 weaponNum, int32 bagNum)
{
	if (weaponNum != -1)
		WeaponAmmo->SetText(FText::AsNumber(weaponNum));

	if (bagNum != -1)
		BagAmmo->SetText(FText::AsNumber(bagNum));
}
void UHUDWidget::UpdateBagNum(EItemType itemType, int32 bagNum)
{
	switch (itemType)
	{
	case EItemType::AMMO:
		BagAmmo->SetText(FText::AsNumber(bagNum));
		break;

	}
}
void UHUDWidget::UpdateWeaponPanel(FWeaponInfo& Info, int32 Idx)
{
	if (Idx == 1)
	{
		Weapon1_Name->SetText(FText::FromString(Info.WeaponName));
		Weapon1_Name->SetRenderOpacity(1.f);
		Weapon2_Name->SetRenderOpacity(0.4f);
	}
	else
	{
		Weapon2_Name->SetText(FText::FromString(Info.WeaponName));
		Weapon2_Name->SetRenderOpacity(1.f);
		Weapon1_Name->SetRenderOpacity(0.4f);
	}

	WeaponAmmo->SetText(FText::AsNumber(Info.Ammo));
	WeaponIcon->SetBrushFromTexture(Info.WeaponIcon);
	WeaponPanel->SetRenderOpacity(1.f);
}

void UHUDWidget::CrosshairTick(float DeltaTime)
{
	if (player != nullptr)
	{
		float currentSpeed = player->GetVelocity().Size();
		float maxSpeed = player->SprintSpeed;
		float hideSpeed = player->WalkSpeed;

		float moveAlpha = FMath::GetMappedRangeValueClamped(FVector2D(0.f, maxSpeed), FVector2D(0.f, 1.f), currentSpeed);
		float hideAlpha = FMath::GetMappedRangeValueClamped(FVector2D(hideSpeed, maxSpeed), FVector2D(0.f, 1.f), currentSpeed);

		if (player->bIsAimming)
			FocusCrosshairAimming(player->ADSAlpha);
		else
		{
			MoveCrosshairByAlpha(moveAlpha);
			HideCrosshairByAlpha(hideAlpha);
		}
	}
}

void UHUDWidget::MoveCrosshairByAlpha(float Alpha)
{
	float MaxMoveLength = 45.f;

	SlotXL->SetPosition(FMath::Lerp(OriginPositionXL, OriginPositionXL + FVector2D(-MaxMoveLength, 0.f), Alpha));
	SlotXR->SetPosition(FMath::Lerp(OriginPositionXR, OriginPositionXR + FVector2D(+MaxMoveLength, 0.f), Alpha));
	SlotYU->SetPosition(FMath::Lerp(OriginPositionYU, OriginPositionYU + FVector2D(0.f, -MaxMoveLength), Alpha));
	SlotYD->SetPosition(FMath::Lerp(OriginPositionYD, OriginPositionYD + FVector2D(0.f, +MaxMoveLength), Alpha));
}

void UHUDWidget::HideCrosshairByAlpha(float Alpha)
{
	CrosshairXL->SetRenderOpacity(1.f - Alpha);
	CrosshairXR->SetRenderOpacity(1.f - Alpha);
	CrosshairYU->SetRenderOpacity(1.f - Alpha);
	CrosshairYD->SetRenderOpacity(1.f - Alpha);
	CrosshairP->SetRenderOpacity(1.f - Alpha);
}

void UHUDWidget::FocusCrosshairAimming(float Alpha)
{
	float FocusMoveLength = -50.f;

	SlotXL->SetPosition(FMath::Lerp(OriginPositionXL, OriginPositionXL + FVector2D(-FocusMoveLength, 0.f), Alpha));
	SlotXR->SetPosition(FMath::Lerp(OriginPositionXR, OriginPositionXR + FVector2D(+FocusMoveLength, 0.f), Alpha));
	SlotYU->SetPosition(FMath::Lerp(OriginPositionYU, OriginPositionYU + FVector2D(0.f, -FocusMoveLength), Alpha));
	SlotYD->SetPosition(FMath::Lerp(OriginPositionYD, OriginPositionYD + FVector2D(0.f, +FocusMoveLength), Alpha));

	HideCrosshairByAlpha(Alpha);
}

void UHUDWidget::PopupHitMark(bool bIsDown)
{
	if (bIsDown)
	{
		HitMarkLD->SetColorAndOpacity(FLinearColor(0.74f, 0.015f, 0.03f, 1.f));
		HitMarkLU->SetColorAndOpacity(FLinearColor(0.74f, 0.015f, 0.03f, 1.f));
		HitMarkRD->SetColorAndOpacity(FLinearColor(0.74f, 0.015f, 0.03f, 1.f));
		HitMarkRU->SetColorAndOpacity(FLinearColor(0.74f, 0.015f, 0.03f, 1.f));
		HitMarkLength = 32.f;
	}
	else
	{
		HitMarkLD->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f));
		HitMarkLU->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f));
		HitMarkRD->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f));
		HitMarkRU->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f));
		HitMarkLength = 16.f;
	}

	HitMarkTimeline.Play();
}

void UHUDWidget::SetPopupValue(float value)
{
	SlotLU->SetSize(FMath::Lerp(OriginSizeHitMark, OriginSizeHitMark + FVector2D(HitMarkLength), value));
	SlotLD->SetSize(FMath::Lerp(OriginSizeHitMark, OriginSizeHitMark + FVector2D(HitMarkLength), value));
	SlotRU->SetSize(FMath::Lerp(OriginSizeHitMark, OriginSizeHitMark + FVector2D(HitMarkLength), value));
	SlotRD->SetSize(FMath::Lerp(OriginSizeHitMark, OriginSizeHitMark + FVector2D(HitMarkLength), value));
}

void UHUDWidget::OnHitPopupFinish()
{
	if (HitMarkTimelineDirection == ETimelineDirection::Forward)
	{
		HitMarkTimeline.Stop();
		auto TimerManager = GetGameInstance()->TimerManager;
		if (TimerManager->IsTimerActive(HitMarkHideTimer))
		{
			TimerManager->ClearTimer(HitMarkHideTimer);
			TimerManager->SetTimer(HitMarkHideTimer, this, &UHUDWidget::ReverseHitMarkTimerline, 0.3f, false);
		}
		else
			TimerManager->SetTimer(HitMarkHideTimer, this, &UHUDWidget::ReverseHitMarkTimerline, 0.3f, false);
	}
}

void UHUDWidget::ReverseHitMarkTimerline()
{
	HitMarkTimeline.Reverse();

}

void UHUDWidget::UpdateInfoPanel(float evo, int32 level, float armourPercent, float healthPercent)
{
	//GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Green, TEXT("update Info"));
	if(level != 4)
		EvoValue->SetText(FText::AsNumber((int)evo));

	ArmourBar->SetPercent(armourPercent);
	HealthBar->SetPercent(healthPercent);

	if (level == ArmourLevel)
		return;
	else
	{
		ArmourLevel = level;

		FSlateBrush bgBrush;
		FSlateBrush fillBrush;
		UTexture2D* bgObj;
		UTexture2D* fillObj;
		UTexture2D* fadeObj;
		UCanvasPanelSlot* barSlot = Cast<UCanvasPanelSlot>(ArmourBar->Slot);
		float BarSizeY = 30.f;
		float BarSizeX = 125.f;

		switch (level)
		{
		case 0:
			ArmourBar->SetRenderOpacity(0.f);
			break;

		case 1:
			barSlot->SetSize(FVector2D(BarSizeX * 2, BarSizeY));
			bgObj = LoadObject<UTexture2D>(NULL, TEXT("Texture2D'/Game/Asset/UMG/ArmourBar_Lv1_NotFill.ArmourBar_Lv1_NotFill'"));
			fillObj = LoadObject<UTexture2D>(NULL, TEXT("Texture2D'/Game/Asset/UMG/ArmourBar_Lv1_Fill.ArmourBar_Lv1_Fill'"));
			fadeObj = LoadObject<UTexture2D>(NULL, TEXT("Texture2D'/Game/Asset/UMG/ShieldEvolution_Lv1.ShieldEvolution_Lv1'"));

			bgBrush.SetResourceObject(bgObj);
			fillBrush.SetResourceObject(fillObj);
			ShieldSphereImg->SetBrushFromTexture(fadeObj);
			ArmourBar->WidgetStyle.SetBackgroundImage(bgBrush);
			ArmourBar->WidgetStyle.SetFillImage(fillBrush);
			ArmourBar->SetRenderOpacity(1.f);
			PlayAnimation(ShieldSphereImgFadeAnim);
			break;

		case 2:
			barSlot->SetSize(FVector2D(BarSizeX * 3, BarSizeY));
			bgObj = LoadObject<UTexture2D>(NULL, TEXT("Texture2D'/Game/Asset/UMG/ArmourBar_Lv2_NotFill.ArmourBar_Lv2_NotFill'"));
			fillObj = LoadObject<UTexture2D>(NULL, TEXT("Texture2D'/Game/Asset/UMG/ArmourBar_Lv2_Fill.ArmourBar_Lv2_Fill'"));
			fadeObj = LoadObject<UTexture2D>(NULL, TEXT("Texture2D'/Game/Asset/UMG/ShieldEvolution_Lv2.ShieldEvolution_Lv2'"));

			bgBrush.SetResourceObject(bgObj);
			fillBrush.SetResourceObject(fillObj);
			ShieldSphereImg->SetBrushFromTexture(fadeObj);
			ArmourBar->WidgetStyle.SetBackgroundImage(bgBrush);
			ArmourBar->WidgetStyle.SetFillImage(fillBrush);
			ArmourBar->SetRenderOpacity(1.f);
			PlayAnimation(ShieldSphereImgFadeAnim);
			break;

		case 3:
			barSlot->SetSize(FVector2D(BarSizeX * 4, BarSizeY));
			bgObj = LoadObject<UTexture2D>(NULL, TEXT("Texture2D'/Game/Asset/UMG/ArmourBar_Lv3_NotFill.ArmourBar_Lv3_NotFill'"));
			fillObj = LoadObject<UTexture2D>(NULL, TEXT("Texture2D'/Game/Asset/UMG/ArmourBar_Lv3_Fill.ArmourBar_Lv3_Fill'"));
			fadeObj = LoadObject<UTexture2D>(NULL, TEXT("Texture2D'/Game/Asset/UMG/ShieldEvolution_Lv3.ShieldEvolution_Lv3'"));

			bgBrush.SetResourceObject(bgObj);
			fillBrush.SetResourceObject(fillObj);
			ShieldSphereImg->SetBrushFromTexture(fadeObj);
			ArmourBar->WidgetStyle.SetBackgroundImage(bgBrush);
			ArmourBar->WidgetStyle.SetFillImage(fillBrush);
			ArmourBar->SetRenderOpacity(1.f);
			PlayAnimation(ShieldSphereImgFadeAnim);
			break;

		case 4:
			barSlot->SetSize(FVector2D(BarSizeX * 5, BarSizeY));
			bgObj = LoadObject<UTexture2D>(NULL, TEXT("Texture2D'/Game/Asset/UMG/ArmourBar_Lv4_NotFill.ArmourBar_Lv4_NotFill'"));
			fillObj = LoadObject<UTexture2D>(NULL, TEXT("Texture2D'/Game/Asset/UMG/ArmourBar_Lv4_Fill.ArmourBar_Lv4_Fill'"));
			fadeObj = LoadObject<UTexture2D>(NULL, TEXT("Texture2D'/Game/Asset/UMG/ShieldEvolution_Lv4.ShieldEvolution_Lv4'"));
			
			bgBrush.SetResourceObject(bgObj);
			fillBrush.SetResourceObject(fillObj);
			ShieldSphereImg->SetBrushFromTexture(fadeObj);
			ArmourBar->WidgetStyle.SetBackgroundImage(bgBrush);
			ArmourBar->WidgetStyle.SetFillImage(fillBrush);
			ArmourBar->SetRenderOpacity(1.f);
			EvoValue->SetText(FText::FromString("MAX"));
			PlayAnimation(ShieldSphereImgFadeAnim);
		}
	}
}