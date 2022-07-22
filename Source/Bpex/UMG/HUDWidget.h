// Fill out your copyright notice in the Description page of Project Settings.
#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Bpex/Object/ItemBase.h"
#include "Bpex/Object/WeaponBase.h"
#include "Components/TimelineComponent.h"
#include "HUDWidget.generated.h"

class UImage;
class UTextBlock;
class UCanvasPanel;
class UCanvasPanelSlot;
class UProgressBar;
class APlayerCharacter;

UCLASS()
class BPEX_API UHUDWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:

	UPROPERTY()
		FTimeline HitMarkTimeline;
	UPROPERTY(BlueprintReadOnly)
		TEnumAsByte<ETimelineDirection::Type> HitMarkTimelineDirection;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		UCurveFloat* HitMarkCurve;

	FTimerHandle HitMarkHideTimer;


	UPROPERTY(meta = (BindWidget))
		UImage* CrosshairP;
	UPROPERTY(meta = (BindWidget))
		UImage* CrosshairXL;
	UPROPERTY(meta = (BindWidget))
		UImage* CrosshairXR;
	UPROPERTY(meta = (BindWidget))
		UImage* CrosshairYU;
	UPROPERTY(meta = (BindWidget))
		UImage* CrosshairYD;
	UPROPERTY(meta = (BindWidget))
		UImage* HitMarkLU;
	UPROPERTY(meta = (BindWidget))
		UImage* HitMarkRU;
	UPROPERTY(meta = (BindWidget))
		UImage* HitMarkLD;
	UPROPERTY(meta = (BindWidget))
		UImage* HitMarkRD;

	UPROPERTY(meta = (BindWidget))
		UTextBlock* WeaponAmmo;
	UPROPERTY(meta = (BindWidget))
		UTextBlock* BagAmmo;
	UPROPERTY(meta = (BindWidget))
		UTextBlock* Weapon1_Name;
	UPROPERTY(meta = (BindWidget))
		UTextBlock* Weapon2_Name;
	UPROPERTY(meta = (BindWidget))
		UImage* WeaponIcon;
	UPROPERTY(meta = (BindWidget))
		UCanvasPanel* WeaponPanel;

	UPROPERTY(meta = (BindWidget))
		UImage* ShieldSphereImg;
	UPROPERTY(meta = (BindWidgetAnim), Transient)
		UWidgetAnimation* ShieldSphereImgFadeAnim;

	UPROPERTY()
		int32 ArmourLevel = 0;
	UPROPERTY(meta = (BindWidget))
		UProgressBar* ArmourBar;
	UPROPERTY(meta = (BindWidget))
		UProgressBar* HealthBar;
	UPROPERTY(meta = (BindWidget))
		UTextBlock* EvoValue;

	UPROPERTY(BlueprintReadWrite)
		APlayerCharacter* player;

	UPROPERTY()
		UCanvasPanelSlot* SlotXL;
	FVector2D OriginPositionXL;
	UPROPERTY()
		UCanvasPanelSlot* SlotXR;
	FVector2D OriginPositionXR;
	UPROPERTY()
		UCanvasPanelSlot* SlotYU;
	FVector2D OriginPositionYU;
	UPROPERTY()
		UCanvasPanelSlot* SlotYD;
	FVector2D OriginPositionYD;

	float HitMarkLength;
	FVector2D OriginSizeHitMark;
	UPROPERTY()
		UCanvasPanelSlot* SlotLU;
	UPROPERTY()
		UCanvasPanelSlot* SlotRU;
	UPROPERTY()
		UCanvasPanelSlot* SlotLD;
	UPROPERTY()
		UCanvasPanelSlot* SlotRD;


public:
	UFUNCTION(BlueprintCallable)
		void SetupWidget();

	UFUNCTION(BlueprintCallable)
		void CrosshairTick(float DeltaTime);

	UFUNCTION()
		void MoveCrosshairByAlpha(float Alpha);
	UFUNCTION()
		void HideCrosshairByAlpha(float Alpha);
	UFUNCTION()
		void FocusCrosshairAimming(float Alpha);
	UFUNCTION(BlueprintCallable)
		void PopupHitMark(bool bIsDown);
	UFUNCTION()
		void SetPopupValue(float value);
	UFUNCTION()
		void OnHitPopupFinish();
	UFUNCTION()
		void ReverseHitMarkTimerline();

	UFUNCTION()
		void UpdateAmmoNums(int32 weaponNum, int32 bagNum);
	UFUNCTION()
		void UpdateBagNum(EItemType itemType, int32 bagNum);
	UFUNCTION()
		void UpdateWeaponPanel(FWeaponInfo& Info, int32 Idx);
	UFUNCTION()
		void UpdateInfoPanel(float evo, int32 level, float armourPercent, float healthPercent);
};
